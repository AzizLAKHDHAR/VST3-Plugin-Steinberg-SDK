//------------------------------------------------------------------------
// MyVST3Plugin: Plugin Factory Implementation
//------------------------------------------------------------------------

#include "pluginprocessor.h"
#include "plugincontroller.h"
#include "pluginids.h"
#include "version.h"

#include "public.sdk/source/main/pluginfactory.h"

#define stringPluginName "MyVST3Plugin"

//------------------------------------------------------------------------
//  VST Plugin Main Entry
//------------------------------------------------------------------------
  
using namespace Steinberg;
using namespace Steinberg::Vst;

BEGIN_FACTORY_DEF(stringCompanyName, stringLegalCopyright, stringLegalTrademarks)

    DEF_CLASS2(INLINE_UID_FROM_FUID(MyVST3Plugin::ProcessorUID),
        PClassInfo::kManyInstances,
        kVstAudioEffectClass,
        stringPluginName,
        kDistributable,
        MyVST3Plugin::kPluginCategory,
        FULL_VERSION_STR,
        kVstVersionString,
        MyVST3Plugin::PluginProcessor::createInstance)

    DEF_CLASS2(INLINE_UID_FROM_FUID(MyVST3Plugin::ControllerUID),
        PClassInfo::kManyInstances,
        kVstComponentControllerClass,
        stringPluginName "Controller",
        0,
        "",
        FULL_VERSION_STR,
        kVstVersionString,
        MyVST3Plugin::PluginController::createInstance)

END_FACTORY