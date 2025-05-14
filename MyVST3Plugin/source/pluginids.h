//------------------------------------------------------------------------
// MyVST3Plugin: Plugin IDs
//------------------------------------------------------------------------

#pragma once

namespace MyVST3Plugin {

// Plugin unique ID - must be unique for each plugin
static const Steinberg::FUID ProcessorUID (0x12345678, 0x12341234, 0x12345678, 0x9ABCDEF0);
static const Steinberg::FUID ControllerUID (0x87654321, 0x43214321, 0xF0DEBC9A, 0x78563412);

// Plugin names
constexpr auto kProcessorName = "MyVST3Plugin";
constexpr auto kControllerName = "MyVST3Plugin Controller";

// Plugin categories
constexpr auto kPluginCategory = "Fx";

// Parameters
enum ParameterIDs {
    kParamVolume = 1000,
    kParamPan = 1001,
    kParamMix = 1002,      // Reverb mix (dry/wet)
    kParamRoomSize = 1003  // Reverb room size
};

} // namespace MyVST3Plugin