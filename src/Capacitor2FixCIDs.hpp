#ifndef _CAPACITOR2_FIX_CIDS_HPP_
#define _CAPACITOR2_FIX_CIDS_HPP_

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace Steinberg {
    enum Capacitor2FixParamID : Vst::ParamID
    {
        kBypass = 100,

        kLowpass = 1000,
        kHighpass = 1001,
        kNonLin = 1002,
        kDryWet = 1003,
    };

    static const FUID Capacitor2FixControllerUID(0x1A2B3C4D, 0x5E6F7A8B, 0x9C0D1E2F, 0x3B4A5C6D);
    static const FUID Capacitor2FixProcessorUID(0x38AF92DE, 0x894A732D, 0x123FA3DB, 0xCABD8305);
};

#endif // _CAPACITOR2_FIX_CIDS_HPP_
