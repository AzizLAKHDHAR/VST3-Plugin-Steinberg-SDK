//------------------------------------------------------------------------
// MyVST3Plugin: Audio Processor Header
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstaudioeffect.h"

#include "pluginids.h"

namespace MyVST3Plugin {

//------------------------------------------------------------------------
// PluginProcessor: Main audio processing class
//------------------------------------------------------------------------
class PluginProcessor : public Steinberg::Vst::AudioEffect
{
public:
    PluginProcessor();
    virtual ~PluginProcessor();

    // Create function
    static Steinberg::FUnknown* createInstance(void* /*context*/)
    {
        return (Steinberg::Vst::IAudioProcessor*)new PluginProcessor;
    }

    // AudioEffect overrides
    Steinberg::tresult PLUGIN_API initialize(Steinberg::FUnknown* context) SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API terminate() SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API setActive(Steinberg::TBool state) SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API process(Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API setState(Steinberg::IBStream* state) SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API getState(Steinberg::IBStream* state) SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API setupProcessing(Steinberg::Vst::ProcessSetup& setup) SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API canProcessSampleSize(Steinberg::int32 symbolicSampleSize) SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API setBusArrangements(Steinberg::Vst::SpeakerArrangement* inputs, 
                                                   Steinberg::int32 numIns,
                                                   Steinberg::Vst::SpeakerArrangement* outputs, 
                                                   Steinberg::int32 numOuts) SMTG_OVERRIDE;

private:
    // Parameters
    float fVolume; // Volume parameter
    float fPan;    // Pan parameter
    float fMix;    // Reverb mix parameter
    float fRoomSize; // Reverb room size parameter

    // Reverb state (simple comb/allpass buffers)
    std::vector<float> reverbBufferL;
    std::vector<float> reverbBufferR;
    int reverbIndex = 0;

    // Processing state
    Steinberg::Vst::ProcessSetup processSetup;
    bool isActive;
};

} // namespace MyVST3Plugin