//------------------------------------------------------------------------
// MyVST3Plugin: Controller Implementation
//------------------------------------------------------------------------

#include "plugincontroller.h"
#include "pluginids.h"
#include "version.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ustring.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include <cstring> // For std::strcmp

namespace MyVST3Plugin {

//------------------------------------------------------------------------
// PluginController Implementation
//------------------------------------------------------------------------
PluginController::PluginController()
{
    // Nothing to do here yet
}

//------------------------------------------------------------------------
PluginController::~PluginController()
{
    // Nothing to do here yet
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API PluginController::initialize(Steinberg::FUnknown* context)
{
    // Call parent implementation first
    Steinberg::tresult result = EditControllerEx1::initialize(context);
    if (result != Steinberg::kResultOk)
    {
        return result;
    }

    // Register parameters
    // Volume parameter (0 to 1, default 1)
    Steinberg::Vst::Parameter* param = new Steinberg::Vst::RangeParameter(
        STR16("Volume"), kParamVolume, STR16("%"),
        0.0, 1.0, 1.0,
        0, Steinberg::Vst::ParameterInfo::kCanAutomate);
    param->setPrecision(2); // Two decimal places
    parameters.addParameter(param);
    
    // Pan parameter (0 = left, 0.5 = center, 1 = right, default 0.5)
    param = new Steinberg::Vst::RangeParameter(
        STR16("Pan"), kParamPan, STR16(""),
        0.0, 1.0, 0.5,
        0, Steinberg::Vst::ParameterInfo::kCanAutomate);
    param->setPrecision(2); // Two decimal places
    parameters.addParameter(param);

    // Mix parameter (0 = dry, 1 = wet, default 0.3)
    param = new Steinberg::Vst::RangeParameter(
        STR16("Mix"), kParamMix, STR16(""),
        0.0, 1.0, 0.3,
        0, Steinberg::Vst::ParameterInfo::kCanAutomate);
    param->setPrecision(2); // Two decimal places
    parameters.addParameter(param);

    // Room Size parameter (0 = small, 1 = large, default 0.5)
    param = new Steinberg::Vst::RangeParameter(
        STR16("Room Size"), kParamRoomSize, STR16(""),
        0.0, 1.0, 0.5,
        0, Steinberg::Vst::ParameterInfo::kCanAutomate);
    param->setPrecision(2); // Two decimal places
    parameters.addParameter(param);

    return Steinberg::kResultOk;
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API PluginController::terminate()
{
    // Nothing to do here yet
    return EditControllerEx1::terminate();
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API PluginController::setComponentState(Steinberg::IBStream* state)
{
    if (!state)
        return Steinberg::kResultFalse;
        
    // Read the processor's state to update our parameters
    Steinberg::IBStreamer streamer(state, kLittleEndian);
    
    float volume = 1.0f;
    float pan = 0.0f;
    
    streamer.readFloat(volume);
    streamer.readFloat(pan);
    
    // Update our parameters
    setParamNormalized(kParamVolume, volume);
    setParamNormalized(kParamPan, (pan + 1.0f) / 2.0f); // Convert [-1, 1] to [0, 1]
    
    return Steinberg::kResultOk;
}

//------------------------------------------------------------------------
Steinberg::IPlugView* PLUGIN_API PluginController::createView(Steinberg::FIDString name)
{
    if (std::strcmp(name, Steinberg::Vst::ViewType::kEditor) == 0)
    {
        // Create a VSTGUI editor
        auto* view = new VSTGUI::VST3Editor(this, "view", "plugin.uidesc");
        return view;
    }
    return nullptr;
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API PluginController::setState(Steinberg::IBStream* state)
{
    return EditControllerEx1::setState(state);
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API PluginController::getState(Steinberg::IBStream* state)
{
    return EditControllerEx1::getState(state);
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API PluginController::setParamNormalized(Steinberg::Vst::ParamID tag, Steinberg::Vst::ParamValue value)
{
    // Set the parameter value
    Steinberg::tresult result = EditControllerEx1::setParamNormalized(tag, value);
    return result;
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API PluginController::getParamStringByValue(Steinberg::Vst::ParamID tag, Steinberg::Vst::ParamValue valueNormalized, 
                                                                  Steinberg::Vst::String128 string)
{
    // Custom string conversion for parameters
    switch (tag)
    {
        case kParamVolume:
        {
            Steinberg::UString128 wrapper(string);
            wrapper.printFloat(valueNormalized * 100.0, 1);
            wrapper.append(STR16(" %"));
            return Steinberg::kResultTrue;
        }
        case kParamPan:
        {
            Steinberg::UString128 wrapper(string);
            if (valueNormalized < 0.5)
            {
                wrapper.printFloat((0.5 - valueNormalized) * 200.0, 1);
                wrapper.append(STR16(" L"));
            }
            else if (valueNormalized > 0.5)
            {
                wrapper.printFloat((valueNormalized - 0.5) * 200.0, 1);
                wrapper.append(STR16(" R"));
            }
            else
            {
                wrapper.fromAscii("Center");
            }
            return Steinberg::kResultTrue;
        }
        case kParamMix:
        {
            Steinberg::UString128 wrapper(string);
            wrapper.printFloat(valueNormalized * 100.0, 1);
            wrapper.append(STR16(" % Wet"));
            return Steinberg::kResultTrue;
        }
        case kParamRoomSize:
        {
            Steinberg::UString128 wrapper(string);
            wrapper.printFloat(10.0 + valueNormalized * 90.0, 0);
            wrapper.append(STR16(" m²"));
            return Steinberg::kResultTrue;
        }
    }
    
    return EditControllerEx1::getParamStringByValue(tag, valueNormalized, string);
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API PluginController::getParamValueByString(Steinberg::Vst::ParamID tag, Steinberg::Vst::TChar* string, 
                                                                  Steinberg::Vst::ParamValue& valueNormalized)
{
    // We don't implement custom string parsing in this example
    return EditControllerEx1::getParamValueByString(tag, string, valueNormalized);
}

} // namespace MyVST3Plugin