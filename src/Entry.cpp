#ifndef _ENTRY_CPP_
#define _ENTRY_CPP_

#include "Capacitor2FixCIDs.hpp"
#include "Capacitor2FixController.hpp"
#include "Capacitor2FixProcessor.hpp"

#include "public.sdk/source/main/pluginfactory.h"

BEGIN_FACTORY_DEF("skarjala", "https://skarja.la", "spencerkarjala@gmail.com")

DEF_CLASS2(INLINE_UID_FROM_FUID(Steinberg::Capacitor2FixProcessorUID),
           Steinberg::PClassInfo::kManyInstances,
           kVstAudioEffectClass,
           "Capacitor2Fix",
           Steinberg::Vst::kDistributable,
           Steinberg::Vst::PlugType::kFxFilter,
           "0.0.1",
           kVstVersionString,
           Steinberg::Vst::Capacitor2FixProcessor::createInstance)

DEF_CLASS2(INLINE_UID_FROM_FUID(Steinberg::Capacitor2FixControllerUID),
           Steinberg::PClassInfo::kManyInstances,
           kVstComponentControllerClass,
           "Capacitor2Fix",
           Steinberg::Vst::kDistributable,
           "",
           "0.0.1",
           kVstVersionString,
           Steinberg::Vst::Capacitor2FixController::createInstance)

END_FACTORY

#endif // _ENTRY_CPP_
