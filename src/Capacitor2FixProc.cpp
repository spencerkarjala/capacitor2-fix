/* ========================================
 *  Capacitor2 - Capacitor2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Capacitor2Fix_H
#include "Capacitor2Fix.h"
#endif

double safeVal(double input) {
    return 100.0 * tanh(input / 100.0);
}

void Capacitor2Fix::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

    lowpassChase = pow(A,2);
    highpassChase = pow(B,2);
    double nonLin = 1.0+((1.0-C)*6.0);
    double nonLinTrim = 1.5/cbrt(nonLin);
    wetChase = D;
    //should not scale with sample rate, because values reaching 1 are important
    //to its ability to bypass when set to max
    double lowpassSpeed = 300 / (fabs( lastLowpass - lowpassChase)+1.0);
    double highpassSpeed = 300 / (fabs( lastHighpass - highpassChase)+1.0);
    double wetSpeed = 300 / (fabs( lastWet - wetChase)+1.0);
    lastLowpass = lowpassChase;
    lastHighpass = highpassChase;
    lastWet = wetChase;
        
    while (--sampleFrames >= 0)
    {
        double inputSampleL = *in1;
        double inputSampleR = *in2;
        if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
        if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
        double drySampleL = inputSampleL;
        double drySampleR = inputSampleR;
        
        double dielectricScaleL = fabs(2.0-((inputSampleL+nonLin)/nonLin));
        double dielectricScaleR = fabs(2.0-((inputSampleR+nonLin)/nonLin));
        
        lowpassBaseAmount = (((lowpassBaseAmount*lowpassSpeed)+lowpassChase)/(lowpassSpeed + 1.0));
        //positive voltage will mean lower capacitance when capacitor is barium titanate
        //on the lowpass, higher pressure means positive swings/smaller cap/larger value for lowpassAmount
        double lowpassAmountL = lowpassBaseAmount * dielectricScaleL;
        double invLowpassL = 1.0 - lowpassAmountL;
        double lowpassAmountR = lowpassBaseAmount * dielectricScaleR;
        double invLowpassR = 1.0 - lowpassAmountR;
        
        highpassBaseAmount = (((highpassBaseAmount*highpassSpeed)+highpassChase)/(highpassSpeed + 1.0));
        //positive voltage will mean lower capacitance when capacitor is barium titanate
        //on the highpass, higher pressure means positive swings/smaller cap/larger value for highpassAmount
        double highpassAmountL = highpassBaseAmount * dielectricScaleL;
        double invHighpassL = 1.0 - highpassAmountL;
        double highpassAmountR = highpassBaseAmount * dielectricScaleR;
        double invHighpassR = 1.0 - highpassAmountR;
        
        wet = (((wet*wetSpeed)+wetChase)/(wetSpeed+1.0));
        
        count++; if (count > 5) count = 0; switch (count)
        {
            case 0:
                iirHighpassAL = safeVal((iirHighpassAL * invHighpassL) + (inputSampleL * highpassAmountL)); inputSampleL -= iirHighpassAL;
                iirLowpassAL = safeVal((iirLowpassAL * invLowpassL) + (inputSampleL * lowpassAmountL)); inputSampleL = iirLowpassAL;
                iirHighpassBL = safeVal((iirHighpassBL * invHighpassL) + (inputSampleL * highpassAmountL)); inputSampleL -= iirHighpassBL;
                iirLowpassBL = safeVal((iirLowpassBL * invLowpassL) + (inputSampleL * lowpassAmountL)); inputSampleL = iirLowpassBL;
                iirHighpassDL = safeVal((iirHighpassDL * invHighpassL) + (inputSampleL * highpassAmountL)); inputSampleL -= iirHighpassDL;
                iirLowpassDL = safeVal((iirLowpassDL * invLowpassL) + (inputSampleL * lowpassAmountL)); inputSampleL = iirLowpassDL;
                iirHighpassAR = safeVal((iirHighpassAR * invHighpassR) + (inputSampleR * highpassAmountR)); inputSampleR -= iirHighpassAR;
                iirLowpassAR = safeVal((iirLowpassAR * invLowpassR) + (inputSampleR * lowpassAmountR)); inputSampleR = iirLowpassAR;
                iirHighpassBR = safeVal((iirHighpassBR * invHighpassR) + (inputSampleR * highpassAmountR)); inputSampleR -= iirHighpassBR;
                iirLowpassBR = safeVal((iirLowpassBR * invLowpassR) + (inputSampleR * lowpassAmountR)); inputSampleR = iirLowpassBR;
                iirHighpassDR = safeVal((iirHighpassDR * invHighpassR) + (inputSampleR * highpassAmountR)); inputSampleR -= iirHighpassDR;
                iirLowpassDR = safeVal((iirLowpassDR * invLowpassR) + (inputSampleR * lowpassAmountR)); inputSampleR = iirLowpassDR;
                break;
            case 1:
                iirHighpassAL = safeVal((iirHighpassAL * invHighpassL) + (inputSampleL * highpassAmountL)); inputSampleL -= iirHighpassAL;
                iirLowpassAL = safeVal((iirLowpassAL * invLowpassL) + (inputSampleL * lowpassAmountL)); inputSampleL = iirLowpassAL;
                iirHighpassCL = safeVal((iirHighpassCL * invHighpassL) + (inputSampleL * highpassAmountL)); inputSampleL -= iirHighpassCL;
                iirLowpassCL = safeVal((iirLowpassCL * invLowpassL) + (inputSampleL * lowpassAmountL)); inputSampleL = iirLowpassCL;
                iirHighpassEL = safeVal((iirHighpassEL * invHighpassL) + (inputSampleL * highpassAmountL)); inputSampleL -= iirHighpassEL;
                iirLowpassEL = safeVal((iirLowpassEL * invLowpassL) + (inputSampleL * lowpassAmountL)); inputSampleL = iirLowpassEL;
                iirHighpassAR = safeVal((iirHighpassAR * invHighpassR) + (inputSampleR * highpassAmountR)); inputSampleR -= iirHighpassAR;
                iirLowpassAR = safeVal((iirLowpassAR * invLowpassR) + (inputSampleR * lowpassAmountR)); inputSampleR = iirLowpassAR;
                iirHighpassCR = safeVal((iirHighpassCR * invHighpassR) + (inputSampleR * highpassAmountR)); inputSampleR -= iirHighpassCR;
                iirLowpassCR = safeVal((iirLowpassCR * invLowpassR) + (inputSampleR * lowpassAmountR)); inputSampleR = iirLowpassCR;
                iirHighpassER = safeVal((iirHighpassER * invHighpassR) + (inputSampleR * highpassAmountR)); inputSampleR -= iirHighpassER;
                iirLowpassER = safeVal((iirLowpassER * invLowpassR) + (inputSampleR * lowpassAmountR)); inputSampleR = iirLowpassER;
            break;
            case 2:
                iirHighpassAL = safeVal((iirHighpassAL * invHighpassL) + (inputSampleL * highpassAmountL)); inputSampleL -= iirHighpassAL;
                iirLowpassAL = safeVal((iirLowpassAL * invLowpassL) + (inputSampleL * lowpassAmountL)); inputSampleL = iirLowpassAL;
                iirHighpassBL = safeVal((iirHighpassBL * invHighpassL) + (inputSampleL * highpassAmountL)); inputSampleL -= iirHighpassBL;
                iirLowpassBL = safeVal((iirLowpassBL * invLowpassL) + (inputSampleL * lowpassAmountL)); inputSampleL = iirLowpassBL;
                iirHighpassFL = safeVal((iirHighpassFL * invHighpassL) + (inputSampleL * highpassAmountL)); inputSampleL -= iirHighpassFL;
                iirLowpassFL = safeVal((iirLowpassFL * invLowpassL) + (inputSampleL * lowpassAmountL)); inputSampleL = iirLowpassFL;
                iirHighpassAR = safeVal((iirHighpassAR * invHighpassR) + (inputSampleR * highpassAmountR)); inputSampleR -= iirHighpassAR;
                iirLowpassAR = safeVal((iirLowpassAR * invLowpassR) + (inputSampleR * lowpassAmountR)); inputSampleR = iirLowpassAR;
                iirHighpassBR = safeVal((iirHighpassBR * invHighpassR) + (inputSampleR * highpassAmountR)); inputSampleR -= iirHighpassBR;
                iirLowpassBR = safeVal((iirLowpassBR * invLowpassR) + (inputSampleR * lowpassAmountR)); inputSampleR = iirLowpassBR;
                iirHighpassFR = safeVal((iirHighpassFR * invHighpassR) + (inputSampleR * highpassAmountR)); inputSampleR -= iirHighpassFR;
                iirLowpassFR = safeVal((iirLowpassFR * invLowpassR) + (inputSampleR * lowpassAmountR)); inputSampleR = iirLowpassFR;
            break;
            case 3:
                iirHighpassAL = safeVal((iirHighpassAL * invHighpassL) + (inputSampleL * highpassAmountL)); inputSampleL -= iirHighpassAL;
                iirLowpassAL = safeVal((iirLowpassAL * invLowpassL) + (inputSampleL * lowpassAmountL)); inputSampleL = iirLowpassAL;
                iirHighpassCL = safeVal((iirHighpassCL * invHighpassL) + (inputSampleL * highpassAmountL)); inputSampleL -= iirHighpassCL;
                iirLowpassCL = safeVal((iirLowpassCL * invLowpassL) + (inputSampleL * lowpassAmountL)); inputSampleL = iirLowpassCL;
                iirHighpassDL = safeVal((iirHighpassDL * invHighpassL) + (inputSampleL * highpassAmountL)); inputSampleL -= iirHighpassDL;
                iirLowpassDL = safeVal((iirLowpassDL * invLowpassL) + (inputSampleL * lowpassAmountL)); inputSampleL = iirLowpassDL;
                iirHighpassAR = safeVal((iirHighpassAR * invHighpassR) + (inputSampleR * highpassAmountR)); inputSampleR -= iirHighpassAR;
                iirLowpassAR = safeVal((iirLowpassAR * invLowpassR) + (inputSampleR * lowpassAmountR)); inputSampleR = iirLowpassAR;
                iirHighpassCR = safeVal((iirHighpassCR * invHighpassR) + (inputSampleR * highpassAmountR)); inputSampleR -= iirHighpassCR;
                iirLowpassCR = safeVal((iirLowpassCR * invLowpassR) + (inputSampleR * lowpassAmountR)); inputSampleR = iirLowpassCR;
                iirHighpassDR = safeVal((iirHighpassDR * invHighpassR) + (inputSampleR * highpassAmountR)); inputSampleR -= iirHighpassDR;
                iirLowpassDR = safeVal((iirLowpassDR * invLowpassR) + (inputSampleR * lowpassAmountR)); inputSampleR = iirLowpassDR;
            break;
            case 4:
                iirHighpassAL = safeVal((iirHighpassAL * invHighpassL) + (inputSampleL * highpassAmountL)); inputSampleL -= iirHighpassAL;
                iirLowpassAL = safeVal((iirLowpassAL * invLowpassL) + (inputSampleL * lowpassAmountL)); inputSampleL = iirLowpassAL;
                iirHighpassBL = safeVal((iirHighpassBL * invHighpassL) + (inputSampleL * highpassAmountL)); inputSampleL -= iirHighpassBL;
                iirLowpassBL = safeVal((iirLowpassBL * invLowpassL) + (inputSampleL * lowpassAmountL)); inputSampleL = iirLowpassBL;
                iirHighpassEL = safeVal((iirHighpassEL * invHighpassL) + (inputSampleL * highpassAmountL)); inputSampleL -= iirHighpassEL;
                iirLowpassEL = safeVal((iirLowpassEL * invLowpassL) + (inputSampleL * lowpassAmountL)); inputSampleL = iirLowpassEL;
                iirHighpassAR = safeVal((iirHighpassAR * invHighpassR) + (inputSampleR * highpassAmountR)); inputSampleR -= iirHighpassAR;
                iirLowpassAR = safeVal((iirLowpassAR * invLowpassR) + (inputSampleR * lowpassAmountR)); inputSampleR = iirLowpassAR;
                iirHighpassBR = safeVal((iirHighpassBR * invHighpassR) + (inputSampleR * highpassAmountR)); inputSampleR -= iirHighpassBR;
                iirLowpassBR = safeVal((iirLowpassBR * invLowpassR) + (inputSampleR * lowpassAmountR)); inputSampleR = iirLowpassBR;
                iirHighpassER = safeVal((iirHighpassER * invHighpassR) + (inputSampleR * highpassAmountR)); inputSampleR -= iirHighpassER;
                iirLowpassER = safeVal((iirLowpassER * invLowpassR) + (inputSampleR * lowpassAmountR)); inputSampleR = iirLowpassER;
            break;
            case 5:
                iirHighpassAL = safeVal((iirHighpassAL * invHighpassL) + (inputSampleL * highpassAmountL)); inputSampleL -= iirHighpassAL;
                iirLowpassAL = safeVal((iirLowpassAL * invLowpassL) + (inputSampleL * lowpassAmountL)); inputSampleL = iirLowpassAL;
                iirHighpassCL = safeVal((iirHighpassCL * invHighpassL) + (inputSampleL * highpassAmountL)); inputSampleL -= iirHighpassCL;
                iirLowpassCL = safeVal((iirLowpassCL * invLowpassL) + (inputSampleL * lowpassAmountL)); inputSampleL = iirLowpassCL;
                iirHighpassFL = safeVal((iirHighpassFL * invHighpassL) + (inputSampleL * highpassAmountL)); inputSampleL -= iirHighpassFL;
                iirLowpassFL = safeVal((iirLowpassFL * invLowpassL) + (inputSampleL * lowpassAmountL)); inputSampleL = iirLowpassFL;
                iirHighpassAR = safeVal((iirHighpassAR * invHighpassR) + (inputSampleR * highpassAmountR)); inputSampleR -= iirHighpassAR;
                iirLowpassAR = safeVal((iirLowpassAR * invLowpassR) + (inputSampleR * lowpassAmountR)); inputSampleR = iirLowpassAR;
                iirHighpassCR = safeVal((iirHighpassCR * invHighpassR) + (inputSampleR * highpassAmountR)); inputSampleR -= iirHighpassCR;
                iirLowpassCR = safeVal((iirLowpassCR * invLowpassR) + (inputSampleR * lowpassAmountR)); inputSampleR = iirLowpassCR;
                iirHighpassFR = safeVal((iirHighpassFR * invHighpassR) + (inputSampleR * highpassAmountR)); inputSampleR -= iirHighpassFR;
                iirLowpassFR = safeVal((iirLowpassFR * invLowpassR) + (inputSampleR * lowpassAmountR)); inputSampleR = iirLowpassFR;
            break;
        }
        //Highpass Filter chunk. This is three poles of IIR highpass, with a 'gearbox' that progressively
        //steepens the filter after minimizing artifacts.
        
        inputSampleL = (drySampleL * (1.0-wet)) + (inputSampleL * nonLinTrim * wet);
        inputSampleR = (drySampleR * (1.0-wet)) + (inputSampleR * nonLinTrim * wet);
        
        // //begin 32 bit stereo floating point dither
        int expon; frexpf((float)inputSampleL, &expon);
        fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
        inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
        frexpf((float)inputSampleR, &expon);
        fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
        inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
        // //end 32 bit stereo floating point dither
        
        *out1 = inputSampleL;
        *out2 = inputSampleR;

        *in1++;
        *in2++;
        *out1++;
        *out2++;
    }
}

void Capacitor2Fix::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

    lowpassChase = pow(A,2);
    highpassChase = pow(B,2);
    double nonLin = 1.0+((1.0-C)*6.0);
    double nonLinTrim = 1.5/cbrt(nonLin);
    wetChase = D;
    //should not scale with sample rate, because values reaching 1 are important
    //to its ability to bypass when set to max
    double lowpassSpeed = 300 / (fabs( lastLowpass - lowpassChase)+1.0);
    double highpassSpeed = 300 / (fabs( lastHighpass - highpassChase)+1.0);
    double wetSpeed = 300 / (fabs( lastWet - wetChase)+1.0);
    lastLowpass = lowpassChase;
    lastHighpass = highpassChase;
    lastWet = wetChase;
    
    while (--sampleFrames >= 0)
    {
        double inputSampleL = *in1;
        double inputSampleR = *in2;
        if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
        if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
        double drySampleL = inputSampleL;
        double drySampleR = inputSampleR;
        
        double dielectricScaleL = fabs(2.0-((inputSampleL+nonLin)/nonLin));
        double dielectricScaleR = fabs(2.0-((inputSampleR+nonLin)/nonLin));
        
        lowpassBaseAmount = (((lowpassBaseAmount*lowpassSpeed)+lowpassChase)/(lowpassSpeed + 1.0));
        //positive voltage will mean lower capacitance when capacitor is barium titanate
        //on the lowpass, higher pressure means positive swings/smaller cap/larger value for lowpassAmount
        double lowpassAmountL = lowpassBaseAmount * dielectricScaleL;
        double invLowpassL = 1.0 - lowpassAmountL;
        double lowpassAmountR = lowpassBaseAmount * dielectricScaleR;
        double invLowpassR = 1.0 - lowpassAmountR;
        
        highpassBaseAmount = (((highpassBaseAmount*highpassSpeed)+highpassChase)/(highpassSpeed + 1.0));
        //positive voltage will mean lower capacitance when capacitor is barium titanate
        //on the highpass, higher pressure means positive swings/smaller cap/larger value for highpassAmount
        double highpassAmountL = highpassBaseAmount * dielectricScaleL;
        double invHighpassL = 1.0 - highpassAmountL;
        double highpassAmountR = highpassBaseAmount * dielectricScaleR;
        double invHighpassR = 1.0 - highpassAmountR;
        
        wet = (((wet*wetSpeed)+wetChase)/(wetSpeed+1.0));
        
        count++; if (count > 5) count = 0; switch (count)
        {
            case 0:
                iirHighpassAL = (iirHighpassAL * invHighpassL) + (inputSampleL * highpassAmountL); inputSampleL -= iirHighpassAL;
                iirLowpassAL = (iirLowpassAL * invLowpassL) + (inputSampleL * lowpassAmountL); inputSampleL = iirLowpassAL;
                iirHighpassBL = (iirHighpassBL * invHighpassL) + (inputSampleL * highpassAmountL); inputSampleL -= iirHighpassBL;
                iirLowpassBL = (iirLowpassBL * invLowpassL) + (inputSampleL * lowpassAmountL); inputSampleL = iirLowpassBL;
                iirHighpassDL = (iirHighpassDL * invHighpassL) + (inputSampleL * highpassAmountL); inputSampleL -= iirHighpassDL;
                iirLowpassDL = (iirLowpassDL * invLowpassL) + (inputSampleL * lowpassAmountL); inputSampleL = iirLowpassDL;
                iirHighpassAR = (iirHighpassAR * invHighpassR) + (inputSampleR * highpassAmountR); inputSampleR -= iirHighpassAR;
                iirLowpassAR = (iirLowpassAR * invLowpassR) + (inputSampleR * lowpassAmountR); inputSampleR = iirLowpassAR;
                iirHighpassBR = (iirHighpassBR * invHighpassR) + (inputSampleR * highpassAmountR); inputSampleR -= iirHighpassBR;
                iirLowpassBR = (iirLowpassBR * invLowpassR) + (inputSampleR * lowpassAmountR); inputSampleR = iirLowpassBR;
                iirHighpassDR = (iirHighpassDR * invHighpassR) + (inputSampleR * highpassAmountR); inputSampleR -= iirHighpassDR;
                iirLowpassDR = (iirLowpassDR * invLowpassR) + (inputSampleR * lowpassAmountR); inputSampleR = iirLowpassDR;
                break;
            case 1:
                iirHighpassAL = (iirHighpassAL * invHighpassL) + (inputSampleL * highpassAmountL); inputSampleL -= iirHighpassAL;
                iirLowpassAL = (iirLowpassAL * invLowpassL) + (inputSampleL * lowpassAmountL); inputSampleL = iirLowpassAL;
                iirHighpassCL = (iirHighpassCL * invHighpassL) + (inputSampleL * highpassAmountL); inputSampleL -= iirHighpassCL;
                iirLowpassCL = (iirLowpassCL * invLowpassL) + (inputSampleL * lowpassAmountL); inputSampleL = iirLowpassCL;
                iirHighpassEL = (iirHighpassEL * invHighpassL) + (inputSampleL * highpassAmountL); inputSampleL -= iirHighpassEL;
                iirLowpassEL = (iirLowpassEL * invLowpassL) + (inputSampleL * lowpassAmountL); inputSampleL = iirLowpassEL;
                iirHighpassAR = (iirHighpassAR * invHighpassR) + (inputSampleR * highpassAmountR); inputSampleR -= iirHighpassAR;
                iirLowpassAR = (iirLowpassAR * invLowpassR) + (inputSampleR * lowpassAmountR); inputSampleR = iirLowpassAR;
                iirHighpassCR = (iirHighpassCR * invHighpassR) + (inputSampleR * highpassAmountR); inputSampleR -= iirHighpassCR;
                iirLowpassCR = (iirLowpassCR * invLowpassR) + (inputSampleR * lowpassAmountR); inputSampleR = iirLowpassCR;
                iirHighpassER = (iirHighpassER * invHighpassR) + (inputSampleR * highpassAmountR); inputSampleR -= iirHighpassER;
                iirLowpassER = (iirLowpassER * invLowpassR) + (inputSampleR * lowpassAmountR); inputSampleR = iirLowpassER;
                break;
            case 2:
                iirHighpassAL = (iirHighpassAL * invHighpassL) + (inputSampleL * highpassAmountL); inputSampleL -= iirHighpassAL;
                iirLowpassAL = (iirLowpassAL * invLowpassL) + (inputSampleL * lowpassAmountL); inputSampleL = iirLowpassAL;
                iirHighpassBL = (iirHighpassBL * invHighpassL) + (inputSampleL * highpassAmountL); inputSampleL -= iirHighpassBL;
                iirLowpassBL = (iirLowpassBL * invLowpassL) + (inputSampleL * lowpassAmountL); inputSampleL = iirLowpassBL;
                iirHighpassFL = (iirHighpassFL * invHighpassL) + (inputSampleL * highpassAmountL); inputSampleL -= iirHighpassFL;
                iirLowpassFL = (iirLowpassFL * invLowpassL) + (inputSampleL * lowpassAmountL); inputSampleL = iirLowpassFL;
                iirHighpassAR = (iirHighpassAR * invHighpassR) + (inputSampleR * highpassAmountR); inputSampleR -= iirHighpassAR;
                iirLowpassAR = (iirLowpassAR * invLowpassR) + (inputSampleR * lowpassAmountR); inputSampleR = iirLowpassAR;
                iirHighpassBR = (iirHighpassBR * invHighpassR) + (inputSampleR * highpassAmountR); inputSampleR -= iirHighpassBR;
                iirLowpassBR = (iirLowpassBR * invLowpassR) + (inputSampleR * lowpassAmountR); inputSampleR = iirLowpassBR;
                iirHighpassFR = (iirHighpassFR * invHighpassR) + (inputSampleR * highpassAmountR); inputSampleR -= iirHighpassFR;
                iirLowpassFR = (iirLowpassFR * invLowpassR) + (inputSampleR * lowpassAmountR); inputSampleR = iirLowpassFR;
                break;
            case 3:
                iirHighpassAL = (iirHighpassAL * invHighpassL) + (inputSampleL * highpassAmountL); inputSampleL -= iirHighpassAL;
                iirLowpassAL = (iirLowpassAL * invLowpassL) + (inputSampleL * lowpassAmountL); inputSampleL = iirLowpassAL;
                iirHighpassCL = (iirHighpassCL * invHighpassL) + (inputSampleL * highpassAmountL); inputSampleL -= iirHighpassCL;
                iirLowpassCL = (iirLowpassCL * invLowpassL) + (inputSampleL * lowpassAmountL); inputSampleL = iirLowpassCL;
                iirHighpassDL = (iirHighpassDL * invHighpassL) + (inputSampleL * highpassAmountL); inputSampleL -= iirHighpassDL;
                iirLowpassDL = (iirLowpassDL * invLowpassL) + (inputSampleL * lowpassAmountL); inputSampleL = iirLowpassDL;
                iirHighpassAR = (iirHighpassAR * invHighpassR) + (inputSampleR * highpassAmountR); inputSampleR -= iirHighpassAR;
                iirLowpassAR = (iirLowpassAR * invLowpassR) + (inputSampleR * lowpassAmountR); inputSampleR = iirLowpassAR;
                iirHighpassCR = (iirHighpassCR * invHighpassR) + (inputSampleR * highpassAmountR); inputSampleR -= iirHighpassCR;
                iirLowpassCR = (iirLowpassCR * invLowpassR) + (inputSampleR * lowpassAmountR); inputSampleR = iirLowpassCR;
                iirHighpassDR = (iirHighpassDR * invHighpassR) + (inputSampleR * highpassAmountR); inputSampleR -= iirHighpassDR;
                iirLowpassDR = (iirLowpassDR * invLowpassR) + (inputSampleR * lowpassAmountR); inputSampleR = iirLowpassDR;
                break;
            case 4:
                iirHighpassAL = (iirHighpassAL * invHighpassL) + (inputSampleL * highpassAmountL); inputSampleL -= iirHighpassAL;
                iirLowpassAL = (iirLowpassAL * invLowpassL) + (inputSampleL * lowpassAmountL); inputSampleL = iirLowpassAL;
                iirHighpassBL = (iirHighpassBL * invHighpassL) + (inputSampleL * highpassAmountL); inputSampleL -= iirHighpassBL;
                iirLowpassBL = (iirLowpassBL * invLowpassL) + (inputSampleL * lowpassAmountL); inputSampleL = iirLowpassBL;
                iirHighpassEL = (iirHighpassEL * invHighpassL) + (inputSampleL * highpassAmountL); inputSampleL -= iirHighpassEL;
                iirLowpassEL = (iirLowpassEL * invLowpassL) + (inputSampleL * lowpassAmountL); inputSampleL = iirLowpassEL;
                iirHighpassAR = (iirHighpassAR * invHighpassR) + (inputSampleR * highpassAmountR); inputSampleR -= iirHighpassAR;
                iirLowpassAR = (iirLowpassAR * invLowpassR) + (inputSampleR * lowpassAmountR); inputSampleR = iirLowpassAR;
                iirHighpassBR = (iirHighpassBR * invHighpassR) + (inputSampleR * highpassAmountR); inputSampleR -= iirHighpassBR;
                iirLowpassBR = (iirLowpassBR * invLowpassR) + (inputSampleR * lowpassAmountR); inputSampleR = iirLowpassBR;
                iirHighpassER = (iirHighpassER * invHighpassR) + (inputSampleR * highpassAmountR); inputSampleR -= iirHighpassER;
                iirLowpassER = (iirLowpassER * invLowpassR) + (inputSampleR * lowpassAmountR); inputSampleR = iirLowpassER;
                break;
            case 5:
                iirHighpassAL = (iirHighpassAL * invHighpassL) + (inputSampleL * highpassAmountL); inputSampleL -= iirHighpassAL;
                iirLowpassAL = (iirLowpassAL * invLowpassL) + (inputSampleL * lowpassAmountL); inputSampleL = iirLowpassAL;
                iirHighpassCL = (iirHighpassCL * invHighpassL) + (inputSampleL * highpassAmountL); inputSampleL -= iirHighpassCL;
                iirLowpassCL = (iirLowpassCL * invLowpassL) + (inputSampleL * lowpassAmountL); inputSampleL = iirLowpassCL;
                iirHighpassFL = (iirHighpassFL * invHighpassL) + (inputSampleL * highpassAmountL); inputSampleL -= iirHighpassFL;
                iirLowpassFL = (iirLowpassFL * invLowpassL) + (inputSampleL * lowpassAmountL); inputSampleL = iirLowpassFL;
                iirHighpassAR = (iirHighpassAR * invHighpassR) + (inputSampleR * highpassAmountR); inputSampleR -= iirHighpassAR;
                iirLowpassAR = (iirLowpassAR * invLowpassR) + (inputSampleR * lowpassAmountR); inputSampleR = iirLowpassAR;
                iirHighpassCR = (iirHighpassCR * invHighpassR) + (inputSampleR * highpassAmountR); inputSampleR -= iirHighpassCR;
                iirLowpassCR = (iirLowpassCR * invLowpassR) + (inputSampleR * lowpassAmountR); inputSampleR = iirLowpassCR;
                iirHighpassFR = (iirHighpassFR * invHighpassR) + (inputSampleR * highpassAmountR); inputSampleR -= iirHighpassFR;
                iirLowpassFR = (iirLowpassFR * invLowpassR) + (inputSampleR * lowpassAmountR); inputSampleR = iirLowpassFR;
                break;
        }
        //Highpass Filter chunk. This is three poles of IIR highpass, with a 'gearbox' that progressively
        //steepens the filter after minimizing artifacts.
        
        inputSampleL = (drySampleL * (1.0-wet)) + (inputSampleL * nonLinTrim * wet);
        inputSampleR = (drySampleR * (1.0-wet)) + (inputSampleR * nonLinTrim * wet);
        
        //begin 64 bit stereo floating point dither
        //int expon; frexp((double)inputSampleL, &expon);
        fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
        //inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
        //frexp((double)inputSampleR, &expon);
        fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
        //inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
        //end 64 bit stereo floating point dither
        
        *out1 = inputSampleL;
        *out2 = inputSampleR;

        *in1++;
        *in2++;
        *out1++;
        *out2++;
    }
}
