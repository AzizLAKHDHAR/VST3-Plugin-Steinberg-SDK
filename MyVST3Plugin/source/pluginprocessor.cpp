//------------------------------------------------------------------------
// MyVST3Plugin: Audio Processor Implementation
//------------------------------------------------------------------------

#include "pluginprocessor.h"
#include "pluginids.h"
#include "version.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

namespace MyVST3Plugin {

//------------------------------------------------------------------------
// PluginProcessor Implementation
//------------------------------------------------------------------------
PluginProcessor::PluginProcessor()
: fVolume(1.0f)
, fPan(0.0f)
, fMix(0.3f) // Default to 30% wet
, fRoomSize(0.5f) // Default medium room
, isActive(false)
, reverbIndex(0)
{
    // Register parameters
    setControllerClass(ControllerUID);
    
    // Initialize processing setup
    memset(&processSetup, 0, sizeof(Steinberg::Vst::ProcessSetup));
}

//------------------------------------------------------------------------
PluginProcessor::~PluginProcessor()
{
    // Nothing to do here yet
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API PluginProcessor::initialize(Steinberg::FUnknown* context)
{
    // Call parent implementation first
    Steinberg::tresult result = AudioEffect::initialize(context);
    if (result != Steinberg::kResultOk)
    {
        return result;
    }

    // Setup audio buses (stereo input and output)
    addAudioInput(STR16("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
    addAudioOutput(STR16("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

    return Steinberg::kResultOk;
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API PluginProcessor::terminate()
{
    // Nothing to do here yet
    return AudioEffect::terminate();
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API PluginProcessor::setActive(Steinberg::TBool state)
{
    isActive = state;
    
    // Reset processing state when activated
    if (state)
    {
        // Reset any processing state here
    }
    
    return Steinberg::kResultOk;
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API PluginProcessor::process(Steinberg::Vst::ProcessData& data)
{
    // Process parameter changes
    if (data.inputParameterChanges)
    {
        Steinberg::int32 numParamsChanged = data.inputParameterChanges->getParameterCount();
        for (Steinberg::int32 i = 0; i < numParamsChanged; i++)
        {
            Steinberg::Vst::IParamValueQueue* paramQueue = data.inputParameterChanges->getParameterData(i);
            if (paramQueue)
            {
                Steinberg::Vst::ParamValue value;
                Steinberg::int32 sampleOffset;
                Steinberg::int32 numPoints = paramQueue->getPointCount();
                
                if (numPoints > 0)
                {
                    // Get the last change value (we don't handle sample accurate automation yet)
                    paramQueue->getPoint(numPoints - 1, sampleOffset, value);
                    
                    switch (paramQueue->getParameterId())
                    {
                        case kParamVolume:
                            fVolume = value;
                            break;
                        case kParamPan:
                            fPan = 2.0f * value - 1.0f; // Convert [0, 1] to [-1, 1]
                            break;
                        case kParamMix:
                            fMix = value;
                            break;
                        case kParamRoomSize:
                            fRoomSize = value;
                            break;
                    }
                }
            }
        }
    }

    // Process audio
    if (data.numInputs == 0 || data.numOutputs == 0 || data.numSamples == 0)
    {
        return Steinberg::kResultOk;
    }

    // Get audio buffers
    Steinberg::Vst::AudioBusBuffers& inputBus = data.inputs[0];
    Steinberg::Vst::AudioBusBuffers& outputBus = data.outputs[0];
    
    // Get channel count
    Steinberg::int32 numChannels = std::min(inputBus.numChannels, outputBus.numChannels);
    
    // Calculate pan values for left and right channels
    float panLeft = (fPan <= 0) ? 1.0f : (1.0f - fPan);
    float panRight = (fPan >= 0) ? 1.0f : (1.0f + fPan);
    
    // Apply volume, pan, and reverb to each sample
    if (data.symbolicSampleSize == Steinberg::Vst::kSample32)
    {
        // Prepare reverb buffer
        int reverbLen = static_cast<int>(processSetup.sampleRate * (0.2 + 1.3 * fRoomSize));
        if (reverbBufferL.size() != reverbLen) {
            reverbBufferL.assign(reverbLen, 0.0f);
            reverbBufferR.assign(reverbLen, 0.0f);
            reverbIndex = 0;
        }
        for (Steinberg::int32 sample = 0; sample < data.numSamples; sample++)
        {
            float inL = inputBus.channelBuffers32[0][sample];
            float inR = inputBus.channelBuffers32[1][sample];
            // Simple feedback comb filter reverb
            float wetL = 0.7f * reverbBufferL[reverbIndex] + 0.3f * inL;
            float wetR = 0.7f * reverbBufferR[reverbIndex] + 0.3f * inR;
            reverbBufferL[reverbIndex] = inL + wetL * 0.5f;
            reverbBufferR[reverbIndex] = inR + wetR * 0.5f;
            reverbIndex = (reverbIndex + 1) % reverbLen;
            float outL = (1.0f - fMix) * inL + fMix * wetL;
            float outR = (1.0f - fMix) * inR + fMix * wetR;
            outputBus.channelBuffers32[0][sample] = outL * fVolume * panLeft;
            outputBus.channelBuffers32[1][sample] = outR * fVolume * panRight;
        }
    }
    else if (data.symbolicSampleSize == Steinberg::Vst::kSample64)
    {
        // Prepare reverb buffer
        int reverbLen = static_cast<int>(processSetup.sampleRate * (0.2 + 1.3 * fRoomSize));
        if (reverbBufferL.size() != reverbLen) {
            reverbBufferL.assign(reverbLen, 0.0f);
            reverbBufferR.assign(reverbLen, 0.0f);
            reverbIndex = 0;
        }
        for (Steinberg::int32 sample = 0; sample < data.numSamples; sample++)
        {
            double inL = inputBus.channelBuffers64[0][sample];
            double inR = inputBus.channelBuffers64[1][sample];
            double wetL = 0.7 * reverbBufferL[reverbIndex] + 0.3 * inL;
            double wetR = 0.7 * reverbBufferR[reverbIndex] + 0.3 * inR;
            reverbBufferL[reverbIndex] = static_cast<float>(inL + wetL * 0.5);
            reverbBufferR[reverbIndex] = static_cast<float>(inR + wetR * 0.5);
            reverbIndex = (reverbIndex + 1) % reverbLen;
            double outL = (1.0 - fMix) * inL + fMix * wetL;
            double outR = (1.0 - fMix) * inR + fMix * wetR;
            outputBus.channelBuffers64[0][sample] = outL * fVolume * panLeft;
            outputBus.channelBuffers64[1][sample] = outR * fVolume * panRight;
        }
    }

    return Steinberg::kResultOk;
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API PluginProcessor::setState(Steinberg::IBStream* state)
{
    if (!state)
        return Steinberg::kResultFalse;
        
    // Read the stored parameters
    Steinberg::IBStreamer streamer(state, kLittleEndian);
    
    streamer.readFloat(fVolume);
    streamer.readFloat(fPan);
    streamer.readFloat(fMix);
    streamer.readFloat(fRoomSize);
    
    return Steinberg::kResultOk;
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API PluginProcessor::getState(Steinberg::IBStream* state)
{
    if (!state)
        return Steinberg::kResultFalse;
        
    // Write the current parameters
    Steinberg::IBStreamer streamer(state, kLittleEndian);
    
    streamer.writeFloat(fVolume);
    streamer.writeFloat(fPan);
    streamer.writeFloat(fMix);
    streamer.writeFloat(fRoomSize);
    
    return Steinberg::kResultOk;
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API PluginProcessor::setupProcessing(Steinberg::Vst::ProcessSetup& setup)
{
    // Store the processing setup
    processSetup = setup;
    return Steinberg::kResultOk;
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API PluginProcessor::canProcessSampleSize(Steinberg::int32 symbolicSampleSize)
{
    // We support both 32-bit and 64-bit processing
    if (symbolicSampleSize == Steinberg::Vst::kSample32 ||
        symbolicSampleSize == Steinberg::Vst::kSample64)
    {
        return Steinberg::kResultOk;
    }
    return Steinberg::kResultFalse;
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API PluginProcessor::setBusArrangements(Steinberg::Vst::SpeakerArrangement* inputs, 
                                                               Steinberg::int32 numIns,
                                                               Steinberg::Vst::SpeakerArrangement* outputs, 
                                                               Steinberg::int32 numOuts)
{
    // We only support stereo in/out
    if (numIns == 1 && numOuts == 1 &&
        inputs[0] == Steinberg::Vst::SpeakerArr::kStereo &&
        outputs[0] == Steinberg::Vst::SpeakerArr::kStereo)
    {
        return Steinberg::kResultOk;
    }
    return Steinberg::kResultFalse;
}

} // namespace MyVST3Plugin