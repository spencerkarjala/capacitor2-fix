// /* ========================================
//  *  Capacitor2
//  *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
//  * ========================================
//  *  Capacitor2Fix - Capacitor2FixProcessor.cpp
//  *  Ported to VST3 by skarjala, open-sourced using GPLv3 license
//  * ======================================== */

#ifndef _CAPACITOR2_FIX_CONTROLLER_
#define _CAPACITOR2_FIX_CONTROLLER_

#include "public.sdk/source/vst/vsteditcontroller.h"

namespace Steinberg {
    namespace Vst {
        class Capacitor2FixController : public EditControllerEx1
        {
          public:
            Capacitor2FixController() = default;
            ~Capacitor2FixController() = default;

            // overrides for IPluginBase
            tresult PLUGIN_API initialize(FUnknown* context) override;
            tresult PLUGIN_API terminate() override;

            // overrides for IEditController (state)
            tresult PLUGIN_API setComponentState(IBStream* state /*in*/) override;
            tresult PLUGIN_API setState(IBStream* state /*in*/) override;
            tresult PLUGIN_API getState(IBStream* state /*inout*/) override;

            IPlugView* PLUGIN_API createView(FIDString name) override;

            tresult PLUGIN_API setParamNormalized(ParamID tag, ParamValue value) override;
            tresult PLUGIN_API getParamStringByValue(ParamID tag, ParamValue valueNormalized, String128 string) override;
            tresult PLUGIN_API getParamValueByString(ParamID tag, TChar* string, ParamValue& valueNormalized) override;

            static FUnknown* createInstance(void*) { return static_cast<IEditController*>(new Capacitor2FixController); }

            DEFINE_INTERFACES
            END_DEFINE_INTERFACES(EditControllerEx1)
            DELEGATE_REFCOUNT(EditControllerEx1)

            //     Capacitor2FixController(audioMasterCallback audioMaster);
            //     ~Capacitor2FixController();
            //     virtual bool getEffectName(char* name);                       // The plug-in name
            //     virtual VstPlugCategory getPlugCategory();                    // The general category for the plug-in
            //     virtual bool getProductString(char* text);                    // This is a unique plug-in string provided by Steinberg
            //     virtual bool getVendorString(char* text);                     // Vendor info
            //     virtual VstInt32 getVendorVersion();                          // Version number
            //     virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
            //     virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);
            //     virtual void getProgramName(char *name);                      // read the name from the host
            //     virtual void setProgramName(char *name);                      // changes the name of the preset displayed in the host
            // 	virtual VstInt32 getChunk (void** data, bool isPreset);
            // 	virtual VstInt32 setChunk (void* data, VstInt32 byteSize, bool isPreset);
            //     virtual float getParameter(VstInt32 index);                   // get the parameter value at the specified index
            //     virtual void setParameter(VstInt32 index, float value);       // set the parameter at index to value
            //     virtual void getParameterLabel(VstInt32 index, char *text);  // label for the parameter (eg dB)
            //     virtual void getParameterName(VstInt32 index, char *text);    // name of the parameter
            //     virtual void getParameterDisplay(VstInt32 index, char *text); // text description of the current value
            //     virtual VstInt32 canDo(char *text);
            // private:
            //     char _programName[kVstMaxProgNameLen + 1];
            //     std::set< std::string > _canDo;

            // 	double iirHighpassAL;
            // 	double iirHighpassBL;
            // 	double iirHighpassCL;
            // 	double iirHighpassDL;
            // 	double iirHighpassEL;
            // 	double iirHighpassFL;
            // 	double iirLowpassAL;
            // 	double iirLowpassBL;
            // 	double iirLowpassCL;
            // 	double iirLowpassDL;
            // 	double iirLowpassEL;
            // 	double iirLowpassFL;

            // 	double iirHighpassAR;
            // 	double iirHighpassBR;
            // 	double iirHighpassCR;
            // 	double iirHighpassDR;
            // 	double iirHighpassER;
            // 	double iirHighpassFR;
            // 	double iirLowpassAR;
            // 	double iirLowpassBR;
            // 	double iirLowpassCR;
            // 	double iirLowpassDR;
            // 	double iirLowpassER;
            // 	double iirLowpassFR;

            // 	int count;

            // 	double lowpassChase;
            // 	double highpassChase;
            // 	double wetChase;

            // 	double lowpassBaseAmount;
            // 	double highpassBaseAmount;
            // 	double wet;

            // 	double lastLowpass;
            // 	double lastHighpass;
            // 	double lastWet;

            // 	uint32_t fpdL;
            // 	uint32_t fpdR;
            // 	//default stuff

            //     float A;
            //     float B;
            //     float C;
            //     float D;
        };
    }
}

#endif // _CAPACITOR2_FIX_CONTROLLER_
