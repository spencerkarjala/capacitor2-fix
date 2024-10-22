// /* ========================================
//  *  Capacitor2
//  *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
//  * ========================================
//  *  Capacitor2Fix - Capacitor2FixProcessor.cpp
//  *  Ported to VST3 by skarjala, open-sourced using GPLv3 license
//  * ======================================== */

#include "Capacitor2FixController.hpp"
#include "Capacitor2FixCIDs.hpp"
#include "base/source/fstreamer.h"

#include <stdexcept>

namespace Steinberg {
    namespace Vst {
        tresult PLUGIN_API Capacitor2FixController::initialize(FUnknown* context)
        {
            if (context == nullptr) {
                return kResultFalse;
            }

            const auto initParentResult = EditControllerEx1::initialize(context);
            if (initParentResult != kResultOk) {
                return initParentResult;
            }

            // clang-format off
            parameters.addParameter(
                STR16("Lowpass"),
                STR16("Capacitor2"),
                256,
                1.0,
                Vst::ParameterInfo::kCanAutomate,
                Capacitor2FixParamID::kLowpass,
                0,
                STR16("Lowpass")
            );
            parameters.addParameter(
                STR16("Highpass"),
                STR16("Capacitor2"),
                256,
                0.0,
                Vst::ParameterInfo::kCanAutomate,
                Capacitor2FixParamID::kHighpass,
                0,
                STR16("Highpass")
            );
            parameters.addParameter(
                STR16("NonLin"),
                STR16("Capacitor2"),
                256,
                0.0,
                Vst::ParameterInfo::kCanAutomate,
                Capacitor2FixParamID::kNonLin,
                0,
                STR16("NonLin")
            );
            parameters.addParameter(
                STR16("Dry/Wet"),
                STR16("Capacitor2"),
                256,
                1.0,
                Vst::ParameterInfo::kCanAutomate,
                Capacitor2FixParamID::kDryWet,
                0,
                STR16("Dry/Wet")
            );
            parameters.addParameter(
                STR16("Bypass"),
                STR16("Capacitor2"),
                1,
                0,
                Vst::ParameterInfo::kCanAutomate | Vst::ParameterInfo::kIsBypass,
                Capacitor2FixParamID::kBypass,
                0,
                STR16("Bypass")
            );
            // clang-format on

            return kResultOk;
        }

        tresult PLUGIN_API Capacitor2FixController::terminate()
        {
            return EditControllerEx1::terminate();
        }

        tresult PLUGIN_API Capacitor2FixController::setComponentState(IBStream* state)
        {
            if (state == nullptr) {
                return kResultFalse;
            }

            IBStreamer streamer(state, kLittleEndian);

            float savedLowpassParam = 0.f;
            if (streamer.readFloat(savedLowpassParam) == false) {
                return kResultFalse;
            }

            float savedHighpassParam = 0.f;
            if (streamer.readFloat(savedHighpassParam) == false) {
                return kResultFalse;
            }

            float savedNonLinParam = 0.f;
            if (streamer.readFloat(savedNonLinParam) == false) {
                return kResultFalse;
            }

            float savedDryWetParam = 0.f;
            if (streamer.readFloat(savedDryWetParam) == false) {
                return kResultFalse;
            }

            int32 savedBypass = 0;
            if (streamer.readInt32(savedBypass) == false) {
                return kResultFalse;
            }

            setParamNormalized(Capacitor2FixParamID::kLowpass, savedLowpassParam);
            setParamNormalized(Capacitor2FixParamID::kHighpass, savedHighpassParam);
            setParamNormalized(Capacitor2FixParamID::kNonLin, savedNonLinParam);
            setParamNormalized(Capacitor2FixParamID::kDryWet, savedDryWetParam);
            setParamNormalized(Capacitor2FixParamID::kBypass, savedBypass);

            return kResultOk;
        }

        tresult PLUGIN_API Capacitor2FixController::setState(IBStream* state)
        {
            if (state == nullptr) {
                return kResultFalse;
            }

            IBStreamer streamer(state, kLittleEndian);

            float savedLowpassParam = 0.f;
            if (streamer.readFloat(savedLowpassParam) == false) {
                return kResultFalse;
            }

            float savedHighpassParam = 0.f;
            if (streamer.readFloat(savedHighpassParam) == false) {
                return kResultFalse;
            }

            float savedNonLinParam = 0.f;
            if (streamer.readFloat(savedNonLinParam) == false) {
                return kResultFalse;
            }

            float savedDryWetParam = 0.f;
            if (streamer.readFloat(savedDryWetParam) == false) {
                return kResultFalse;
            }

            int32 savedBypass = 0;
            if (streamer.readInt32(savedBypass) == false) {
                return kResultFalse;
            }

            setParamNormalized(Capacitor2FixParamID::kLowpass, savedLowpassParam);
            setParamNormalized(Capacitor2FixParamID::kHighpass, savedHighpassParam);
            setParamNormalized(Capacitor2FixParamID::kNonLin, savedNonLinParam);
            setParamNormalized(Capacitor2FixParamID::kDryWet, savedDryWetParam);
            setParamNormalized(Capacitor2FixParamID::kBypass, savedBypass);
            return kResultOk;
        }

        tresult PLUGIN_API Capacitor2FixController::getState(IBStream* state)
        {
            if (state == nullptr) {
                return kResultFalse;
            }

            float toSaveLowpassParam = getParamNormalized(Capacitor2FixParamID::kLowpass);
            float toSaveHighpassParam = getParamNormalized(Capacitor2FixParamID::kHighpass);
            float toSaveNonLinParam = getParamNormalized(Capacitor2FixParamID::kNonLin);
            float toSaveDryWetParam = getParamNormalized(Capacitor2FixParamID::kDryWet);
            float toSaveBypass = getParamNormalized(Capacitor2FixParamID::kBypass);

            IBStreamer streamer(state, kLittleEndian);

            streamer.writeFloat(toSaveLowpassParam);
            streamer.writeFloat(toSaveHighpassParam);
            streamer.writeFloat(toSaveNonLinParam);
            streamer.writeFloat(toSaveDryWetParam);
            streamer.writeInt32(toSaveBypass);

            return kResultOk;
        }

        IPlugView* PLUGIN_API Capacitor2FixController::createView(FIDString name)
        {
            return nullptr;
        }

        tresult PLUGIN_API Capacitor2FixController::setParamNormalized(ParamID tag, ParamValue value)
        {
            // throw std::runtime_error(std::format("11111 {}", value));
            return EditControllerEx1::setParamNormalized(tag, value);
        }

        tresult PLUGIN_API Capacitor2FixController::getParamStringByValue(ParamID tag, ParamValue valueNormalized, String128 string)
        {
            return EditControllerEx1::getParamStringByValue(tag, valueNormalized, string);
        }

        tresult PLUGIN_API Capacitor2FixController::getParamValueByString(ParamID tag, TChar* string, ParamValue& valueNormalized)
        {
            return EditControllerEx1::getParamValueByString(tag, string, valueNormalized);
        }
    }
}

// AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Capacitor2FixController(audioMaster);}

// Capacitor2FixController::Capacitor2FixController(audioMasterCallback audioMaster) :
//     AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
// {
// 	A = 1.0;
// 	B = 0.0;
// 	C = 0.0;
// 	D = 1.0;
// 	iirHighpassAL = 0.0;
// 	iirHighpassBL = 0.0;
// 	iirHighpassCL = 0.0;
// 	iirHighpassDL = 0.0;
// 	iirHighpassEL = 0.0;
// 	iirHighpassFL = 0.0;
// 	iirLowpassAL = 0.0;
// 	iirLowpassBL = 0.0;
// 	iirLowpassCL = 0.0;
// 	iirLowpassDL = 0.0;
// 	iirLowpassEL = 0.0;
// 	iirLowpassFL = 0.0;

// 	iirHighpassAR = 0.0;
// 	iirHighpassBR = 0.0;
// 	iirHighpassCR = 0.0;
// 	iirHighpassDR = 0.0;
// 	iirHighpassER = 0.0;
// 	iirHighpassFR = 0.0;
// 	iirLowpassAR = 0.0;
// 	iirLowpassBR = 0.0;
// 	iirLowpassCR = 0.0;
// 	iirLowpassDR = 0.0;
// 	iirLowpassER = 0.0;
// 	iirLowpassFR = 0.0;
// 	count = 0;
// 	lowpassChase = 0.0;
// 	highpassChase = 0.0;
// 	wetChase = 0.0;
// 	lowpassBaseAmount = 1.0;
// 	highpassBaseAmount = 0.0;
// 	wet = 1.0;
// 	lastLowpass = 1000.0;
// 	lastHighpass = 1000.0;
// 	lastWet = 1000.0;

// 	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
// 	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
// 	//this is reset: values being initialized only once. Startup values, whatever they are.

//     _canDo.insert("plugAsChannelInsert"); // plug-in can be used as a channel insert effect.
//     _canDo.insert("plugAsSend"); // plug-in can be used as a send effect.
//     _canDo.insert("x2in2out");
//     setNumInputs(kNumInputs);
//     setNumOutputs(kNumOutputs);
//     setUniqueID(kUniqueId);
//     canProcessReplacing();     // supports output replacing
//     canDoubleReplacing();      // supports double precision processing
// 	programsAreChunks(true);
//     vst_strncpy (_programName, "Default", kVstMaxProgNameLen); // default program name
// }

// Capacitor2FixController::~Capacitor2FixController() {}
// VstInt32 Capacitor2FixController::getVendorVersion () {return 1000;}
// void Capacitor2FixController::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
// void Capacitor2FixController::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
// //airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
// //trying to do versioning and preventing people from using older versions. Maybe they like the old one!

// static float pinParameter(float data)
// {
// 	if (data < 0.0f) return 0.0f;
// 	if (data > 1.0f) return 1.0f;
// 	return data;
// }

// VstInt32 Capacitor2FixController::getChunk (void** data, bool isPreset)
// {
// 	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
// 	chunkData[0] = A;
// 	chunkData[1] = B;
// 	chunkData[2] = C;
// 	chunkData[3] = D;
// 	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
// 	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you
// 	 started with. */

// 	*data = chunkData;
// 	return kNumParameters * sizeof(float);
// }

// VstInt32 Capacitor2FixController::setChunk (void* data, VstInt32 byteSize, bool isPreset)
// {
// 	float *chunkData = (float *)data;
// 	A = pinParameter(chunkData[0]);
// 	B = pinParameter(chunkData[1]);
// 	C = pinParameter(chunkData[2]);
// 	D = pinParameter(chunkData[3]);
// 	/* We're ignoring byteSize as we found it to be a filthy liar */

// 	/* calculate any other fields you need here - you could copy in
// 	 code from setParameter() here. */
// 	return 0;
// }

// void Capacitor2FixController::setParameter(VstInt32 index, float value) {
//     switch (index) {
//         case kParamA: A = value; break;
//         case kParamB: B = value; break;
//         case kParamC: C = value; break;
//         case kParamD: D = value; break;
//         default: throw; // unknown parameter, shouldn't happen!
//     }
// }

// float Capacitor2FixController::getParameter(VstInt32 index) {
//     switch (index) {
//         case kParamA: return A; break;
//         case kParamB: return B; break;
//         case kParamC: return C; break;
//         case kParamD: return D; break;
//         default: break; // unknown parameter, shouldn't happen!
//     } return 0.0; //we only need to update the relevant name, this is simple to manage
// }

// void Capacitor2FixController::getParameterName(VstInt32 index, char *text) {
//     switch (index) {
//         case kParamA: vst_strncpy (text, "Lowpass", kVstMaxParamStrLen); break;
// 		case kParamB: vst_strncpy (text, "Highpass", kVstMaxParamStrLen); break;
// 		case kParamC: vst_strncpy (text, "NonLin", kVstMaxParamStrLen); break;
// 		case kParamD: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
//         default: break; // unknown parameter, shouldn't happen!
//     } //this is our labels for displaying in the VST host
// }

// void Capacitor2FixController::getParameterDisplay(VstInt32 index, char *text) {
//     switch (index) {
//         case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
//         case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
//         case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
//         case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
//         default: break; // unknown parameter, shouldn't happen!
// 	} //this displays the values and handles 'popups' where it's discrete choices
// }

// void Capacitor2FixController::getParameterLabel(VstInt32 index, char *text) {
//     switch (index) {
//         case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
//         case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
//         case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
//         case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
// 		default: break; // unknown parameter, shouldn't happen!
//     }
// }

// VstInt32 Capacitor2FixController::canDo(char *text)
// { return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

// bool Capacitor2FixController::getEffectName(char* name) {
//     vst_strncpy(name, "Capacitor2FixController", kVstMaxProductStrLen); return true;
// }

// VstPlugCategory Capacitor2FixController::getPlugCategory() {return kPlugCategEffect;}

// bool Capacitor2FixController::getProductString(char* text) {
//   	vst_strncpy (text, "skarjala Capacitor2FixController", kVstMaxProductStrLen); return true;
// }

// bool Capacitor2FixController::getVendorString(char* text) {
//   	vst_strncpy (text, "skarjala", kVstMaxVendorStrLen); return true;
// }
