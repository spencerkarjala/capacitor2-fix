// /* ========================================
//  *  Capacitor2
//  *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
//  * ========================================
//  *  Capacitor2Fix - Capacitor2FixProcessor.cpp
//  *  Ported to VST3 by skarjala, open-sourced using GPLv3 license
//  * ======================================== */

// created with much help from https://github.com/steinbergmedia/vst3_example_plugin_hello_world/

#include "Capacitor2FixProcessor.hpp"
#include "Capacitor2FixCIDs.hpp"
#include "LookupTable.hpp"
#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

#include <cassert>
#include <cmath>
#include <format>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <utility>

namespace Steinberg {
    namespace Vst {
        tresult PLUGIN_API Capacitor2FixProcessor::initialize(FUnknown* context)
        {
            if (context == nullptr) {
                return kResultFalse;
            }

            const auto parentInitResult = AudioEffect::initialize(context);
            if (parentInitResult != kResultOk) {
                return parentInitResult;
            }

            addAudioInput(STR16("Stereo In"), SpeakerArr::kStereo);
            addAudioOutput(STR16("Stereo Out"), SpeakerArr::kStereo);

            setControllerClass(Capacitor2FixControllerUID);

            return kResultOk;
        }

        tresult PLUGIN_API Capacitor2FixProcessor::terminate()
        {
            return AudioEffect::terminate();
        }

        tresult PLUGIN_API Capacitor2FixProcessor::setActive(TBool isActive)
        {
            if (isActive == false) {
                // handling for deactivation of plugin
            } else {
                // handling for activation of plugin
            }

            return kResultOk;
        }

        tresult PLUGIN_API Capacitor2FixProcessor::canProcessSampleSize(int32 symbolicSampleSize)
        {
            if (symbolicSampleSize == Vst::kSample32 || symbolicSampleSize == Vst::kSample64) {
                return kResultOk;
            }
            return kResultFalse;
        }

        tresult PLUGIN_API Capacitor2FixProcessor::setState(IBStream* state)
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

            paramLowpass = savedLowpassParam;
            paramHighpass = savedHighpassParam;
            paramNonLin = savedNonLinParam;
            paramDryWet = savedDryWetParam;
            pluginBypass = savedBypass;

            return kResultOk;
        }

        tresult PLUGIN_API Capacitor2FixProcessor::getState(IBStream* state)
        {
            if (state == nullptr) {
                return kResultFalse;
            }

            const float toSaveLowpassParam = paramLowpass;
            const float toSaveHighpassParam = paramHighpass;
            const float toSaveNonLinParam = paramNonLin;
            const float toSaveDryWetParam = paramDryWet;
            const float toSaveBypass = pluginBypass;

            IBStreamer streamer(state, kLittleEndian);

            streamer.writeFloat(toSaveLowpassParam);
            streamer.writeFloat(toSaveHighpassParam);
            streamer.writeFloat(toSaveNonLinParam);
            streamer.writeFloat(toSaveDryWetParam);
            streamer.writeInt32(toSaveBypass);

            return kResultOk;
        }

        tresult PLUGIN_API Capacitor2FixProcessor::setupProcessing(ProcessSetup& newSetup)
        {
            return AudioEffect::setupProcessing(newSetup);
        }

        double safeVal(double input)
        {
            return 100.0 * std::tanh(input / 100.0);
        }

        tresult PLUGIN_API Capacitor2FixProcessor::process(ProcessData& data)
        {
            if (data.numSamples <= 0) {
                return kResultTrue;
            }
            // handleProcessParamChanges(data.inputParameterChanges, data.numSamples);
            // const auto* paramChanges = data.inputParameterChanges;

            auto* paramChanges = data.inputParameterChanges;
            if (paramChanges != nullptr) {
                auto nParamsChanged = paramChanges->getParameterCount();
                for (int index = 0; index < nParamsChanged; ++index) {
                    auto* paramQueue = paramChanges->getParameterData(index);
                    if (paramQueue == nullptr) {
                        continue;
                    }

                    ParamValue value;
                    int32 sampleOffset;
                    int32 numPoints = paramQueue->getPointCount();

                    const auto getPointResult = paramQueue->getPoint(numPoints - 1, sampleOffset, value);
                    if (getPointResult != kResultTrue) {
                        continue;
                    }

                    switch (paramQueue->getParameterId()) {
                        case Capacitor2FixParamID::kLowpass:
                            paramLowpass = value;
                            break;
                        case Capacitor2FixParamID::kHighpass:
                            paramHighpass = value;
                            break;
                        case Capacitor2FixParamID::kNonLin:
                            paramNonLin = value;
                            break;
                        case Capacitor2FixParamID::kDryWet:
                            paramDryWet = value;
                            break;
                        case Capacitor2FixParamID::kBypass:
                            pluginBypass = value;
                            break;
                    }
                }
            }

            processAudio(data.inputs, data.numInputs, data.outputs, data.numOutputs, data.numSamples);

            return kResultOk;
        }

        void Capacitor2FixProcessor::processAudio(AudioBusBuffers* inputs,
                                                  const int32 nInputs,
                                                  AudioBusBuffers* outputs,
                                                  const int32 nOutputs,
                                                  const int32 nSamples)
        {
            if (inputs == nullptr || outputs == nullptr) {
                return;
            }

            auto nChannels = std::min(inputs->numChannels, outputs->numChannels);
            if (nChannels == 0) {
                return;
            }

            if (nChannels > filters.size()) {
                filters.resize(nChannels);
            }
            if (nChannels > floatingPointDithers.size()) {
                floatingPointDithers.resize(nChannels);
            }

            lowpassChase = pow(paramLowpass, 2);
            highpassChase = pow(paramHighpass, 2);
            double nonLin = 1.0 + ((1.0 - paramNonLin) * 6.0);
            double nonLinTrim = 1.5 / cbrt(nonLin);
            wetChase = paramDryWet;
            // should not scale with sample rate, because values reaching 1 are important
            // to its ability to bypass when set to max
            double lowpassSpeed = 300 / (fabs(lastLowpass - lowpassChase) + 1.0);
            double highpassSpeed = 300 / (fabs(lastHighpass - highpassChase) + 1.0);
            double wetSpeed = 300 / (fabs(lastWet - wetChase) + 1.0);
            lastLowpass = lowpassChase;
            lastHighpass = highpassChase;
            lastWet = wetChase;

            for (int32 channelIndex = 0; channelIndex < nChannels; ++channelIndex) {
                Sample32* in = inputs->channelBuffers32[channelIndex];
                Sample32* out = outputs->channelBuffers32[channelIndex];
                uint32_t& fpd = floatingPointDithers[channelIndex];
                for (int32 sampleIndex = 0; sampleIndex < nSamples; ++sampleIndex) {

                    double inputSample = in[sampleIndex];
                    if (fabs(inputSample) < 1.18e-23) {
                        inputSample = fpd * 1.18e-17;
                    }

                    double drySample = inputSample;
                    double dielectricScale = fabs(2.0 - ((inputSample + nonLin) / nonLin));

                    lowpassBaseAmount = (((lowpassBaseAmount * lowpassSpeed) + lowpassChase) / (lowpassSpeed + 1.0));
                    // positive voltage will mean lower capacitance when capacitor is barium titanate
                    // on the lowpass, higher pressure means positive swings/smaller cap/larger value for lowpassAmount
                    double lowpassAmount = lowpassBaseAmount * dielectricScale;
                    double invLowpass = 1.0 - lowpassAmount;

                    highpassBaseAmount = (((highpassBaseAmount * highpassSpeed) + highpassChase) / (highpassSpeed + 1.0));
                    // positive voltage will mean lower capacitance when capacitor is barium titanate
                    // on the highpass, higher pressure means positive swings/smaller cap/larger value for highpassAmount
                    double highpassAmount = highpassBaseAmount * dielectricScale;
                    double invHighpass = 1.0 - highpassAmount;

                    wet = (((wet * wetSpeed) + wetChase) / (wetSpeed + 1.0));

                    Sample64& iirHighpassA = filters[channelIndex].highpass.coefficients[0];
                    Sample64& iirHighpassB = filters[channelIndex].highpass.coefficients[1];
                    Sample64& iirHighpassC = filters[channelIndex].highpass.coefficients[2];
                    Sample64& iirHighpassD = filters[channelIndex].highpass.coefficients[3];
                    Sample64& iirHighpassE = filters[channelIndex].highpass.coefficients[4];
                    Sample64& iirHighpassF = filters[channelIndex].highpass.coefficients[5];

                    Sample64& iirLowpassA = filters[channelIndex].lowpass.coefficients[0];
                    Sample64& iirLowpassB = filters[channelIndex].lowpass.coefficients[1];
                    Sample64& iirLowpassC = filters[channelIndex].lowpass.coefficients[2];
                    Sample64& iirLowpassD = filters[channelIndex].lowpass.coefficients[3];
                    Sample64& iirLowpassE = filters[channelIndex].lowpass.coefficients[4];
                    Sample64& iirLowpassF = filters[channelIndex].lowpass.coefficients[5];

                    count = (count + 1) % 6;
                    switch (count) {
                        case 0:
                            iirHighpassA = safeVal((iirHighpassA * invHighpass) + (inputSample * highpassAmount));
                            inputSample -= iirHighpassA;
                            iirLowpassA = safeVal((iirLowpassA * invLowpass) + (inputSample * lowpassAmount));
                            inputSample = iirLowpassA;
                            iirHighpassB = safeVal((iirHighpassB * invHighpass) + (inputSample * highpassAmount));
                            inputSample -= iirHighpassB;
                            iirLowpassB = safeVal((iirLowpassB * invLowpass) + (inputSample * lowpassAmount));
                            inputSample = iirLowpassB;
                            iirHighpassD = safeVal((iirHighpassD * invHighpass) + (inputSample * highpassAmount));
                            inputSample -= iirHighpassD;
                            iirLowpassD = safeVal((iirLowpassD * invLowpass) + (inputSample * lowpassAmount));
                            inputSample = iirLowpassD;
                            break;
                        case 1:
                            iirHighpassA = safeVal((iirHighpassA * invHighpass) + (inputSample * highpassAmount));
                            inputSample -= iirHighpassA;
                            iirLowpassA = safeVal((iirLowpassA * invLowpass) + (inputSample * lowpassAmount));
                            inputSample = iirLowpassA;
                            iirHighpassC = safeVal((iirHighpassC * invHighpass) + (inputSample * highpassAmount));
                            inputSample -= iirHighpassC;
                            iirLowpassC = safeVal((iirLowpassC * invLowpass) + (inputSample * lowpassAmount));
                            inputSample = iirLowpassC;
                            iirHighpassE = safeVal((iirHighpassE * invHighpass) + (inputSample * highpassAmount));
                            inputSample -= iirHighpassE;
                            iirLowpassE = safeVal((iirLowpassE * invLowpass) + (inputSample * lowpassAmount));
                            inputSample = iirLowpassE;
                            break;
                        case 2:
                            iirHighpassA = safeVal((iirHighpassA * invHighpass) + (inputSample * highpassAmount));
                            inputSample -= iirHighpassA;
                            iirLowpassA = safeVal((iirLowpassA * invLowpass) + (inputSample * lowpassAmount));
                            inputSample = iirLowpassA;
                            iirHighpassB = safeVal((iirHighpassB * invHighpass) + (inputSample * highpassAmount));
                            inputSample -= iirHighpassB;
                            iirLowpassB = safeVal((iirLowpassB * invLowpass) + (inputSample * lowpassAmount));
                            inputSample = iirLowpassB;
                            iirHighpassF = safeVal((iirHighpassF * invHighpass) + (inputSample * highpassAmount));
                            inputSample -= iirHighpassF;
                            iirLowpassF = safeVal((iirLowpassF * invLowpass) + (inputSample * lowpassAmount));
                            inputSample = iirLowpassF;
                            break;
                        case 3:
                            iirHighpassA = safeVal((iirHighpassA * invHighpass) + (inputSample * highpassAmount));
                            inputSample -= iirHighpassA;
                            iirLowpassA = safeVal((iirLowpassA * invLowpass) + (inputSample * lowpassAmount));
                            inputSample = iirLowpassA;
                            iirHighpassC = safeVal((iirHighpassC * invHighpass) + (inputSample * highpassAmount));
                            inputSample -= iirHighpassC;
                            iirLowpassC = safeVal((iirLowpassC * invLowpass) + (inputSample * lowpassAmount));
                            inputSample = iirLowpassC;
                            iirHighpassD = safeVal((iirHighpassD * invHighpass) + (inputSample * highpassAmount));
                            inputSample -= iirHighpassD;
                            iirLowpassD = safeVal((iirLowpassD * invLowpass) + (inputSample * lowpassAmount));
                            inputSample = iirLowpassD;
                            break;
                        case 4:
                            iirHighpassA = safeVal((iirHighpassA * invHighpass) + (inputSample * highpassAmount));
                            inputSample -= iirHighpassA;
                            iirLowpassA = safeVal((iirLowpassA * invLowpass) + (inputSample * lowpassAmount));
                            inputSample = iirLowpassA;
                            iirHighpassB = safeVal((iirHighpassB * invHighpass) + (inputSample * highpassAmount));
                            inputSample -= iirHighpassB;
                            iirLowpassB = safeVal((iirLowpassB * invLowpass) + (inputSample * lowpassAmount));
                            inputSample = iirLowpassB;
                            iirHighpassE = safeVal((iirHighpassE * invHighpass) + (inputSample * highpassAmount));
                            inputSample -= iirHighpassE;
                            iirLowpassE = safeVal((iirLowpassE * invLowpass) + (inputSample * lowpassAmount));
                            inputSample = iirLowpassE;
                            break;
                        case 5:
                            iirHighpassA = safeVal((iirHighpassA * invHighpass) + (inputSample * highpassAmount));
                            inputSample -= iirHighpassA;
                            iirLowpassA = safeVal((iirLowpassA * invLowpass) + (inputSample * lowpassAmount));
                            inputSample = iirLowpassA;
                            iirHighpassC = safeVal((iirHighpassC * invHighpass) + (inputSample * highpassAmount));
                            inputSample -= iirHighpassC;
                            iirLowpassC = safeVal((iirLowpassC * invLowpass) + (inputSample * lowpassAmount));
                            inputSample = iirLowpassC;
                            iirHighpassF = safeVal((iirHighpassF * invHighpass) + (inputSample * highpassAmount));
                            inputSample -= iirHighpassF;
                            iirLowpassF = safeVal((iirLowpassF * invLowpass) + (inputSample * lowpassAmount));
                            inputSample = iirLowpassF;
                            break;
                    }
                    // Highpass Filter chunk. This is three poles of IIR highpass, with a 'gearbox' that progressively
                    // steepens the filter after minimizing artifacts.

                    inputSample = (drySample * (1.0 - wet)) + (inputSample * nonLinTrim * wet);

                    // //begin 32 bit stereo floating point dither
                    int expon;
                    frexpf((float)inputSample, &expon);
                    fpd ^= fpd << 13;
                    fpd ^= fpd >> 17;
                    fpd ^= fpd << 5;
                    inputSample += ((double(fpd) - uint32_t(0x7fffffff)) * 5.5e-36l * pow(2, expon + 62));
                    // //end 32 bit stereo floating point dither

                    out[sampleIndex] = inputSample;
                }
            }

            // void Capacitor2FixProcessor::handleProcessParamChanges(IParameterChanges* paramChanges, const int32 numSamples)
            // {
            //     if (paramChanges == nullptr) {
            //         return;
            //     }

            //     std::unordered_map<ParamID, skarjala::LookupTable> paramChangesMap;

            //     const auto nChanged = paramChanges->getParameterCount();
            //     for (int index = 0; index < nChanged; ++index) {
            //         auto* paramQueue = paramChanges->getParameterData(index);

            //         const auto paramID = paramQueue->getParameterId();
            //         const auto nParamValueChanges = paramQueue->getPointCount();

            //         std::vector<std::pair<uint32_t, ParamValue>> currParamChanges;
            //         for (int changeIndex = 0; changeIndex < nParamValueChanges; ++changeIndex) {
            //             int32 sampleOffset;
            //             ParamValue changeValue;

            //             const auto getPointResult = paramQueue->getPoint(changeIndex, sampleOffset, changeValue);
            //             if (getPointResult != kResultOk) {
            //                 throw std::runtime_error("process() failed to retrieve a parameter change point");
            //             }

            //             assert(sampleOffset >= 0);
            //             currParamChanges.emplace_back(static_cast<uint32_t>(sampleOffset), changeIndex);
            //         }

            //         if (currParamChanges.front().first != 0) {
            //             currParamChanges.insert(currParamChanges.begin(), { 0, paramValueFromID(paramID) });
            //         }

            //         throw std::runtime_error(std::format("{}", numSamples));
            //         paramChangesMap.emplace(paramID, skarjala::LookupTable(currParamChanges, numSamples));
            //     }

            //     std::cout << "here" << std::endl;
            // }
        }
    }
}
