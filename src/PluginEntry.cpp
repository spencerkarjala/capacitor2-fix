#ifndef _ENTRY_CPP_
#define _ENTRY_CPP_

#include "PluginCIDs.hpp"
#include "PluginController.hpp"
#include "PluginProcessor.hpp"

#include "public.sdk/source/main/pluginfactory.h"

BEGIN_FACTORY_DEF("skarjala", "https://skarja.la", "spencerkarjala@gmail.com")

DEF_CLASS2(INLINE_UID_FROM_FUID(Steinberg::Capacitor2TweakProcessorUID),
           Steinberg::PClassInfo::kManyInstances,
           kVstAudioEffectClass,
           "Capacitor2Tweak",
           Steinberg::Vst::kDistributable,
           Steinberg::Vst::PlugType::kFxFilter,
           "0.0.1",
           kVstVersionString,
           Steinberg::Vst::Capacitor2TweakProcessor::createInstance)

DEF_CLASS2(INLINE_UID_FROM_FUID(Steinberg::Capacitor2TweakControllerUID),
           Steinberg::PClassInfo::kManyInstances,
           kVstComponentControllerClass,
           "Capacitor2Tweak",
           Steinberg::Vst::kDistributable,
           "",
           "0.0.1",
           kVstVersionString,
           Steinberg::Vst::Capacitor2TweakController::createInstance)

END_FACTORY

#endif // _ENTRY_CPP_
