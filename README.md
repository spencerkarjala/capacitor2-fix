# Capacitor2Fix

This repository implements a simple fix for Airwindows' Capacitor2 VST plugin. In some conditions, the original Capacitor2 plugin can produce undesirable outputs (eg. NaNs) that are not typically handled gracefully by DAWs. This version of the plugin fixes that issue.

Most of the effort in this repository was originally done by Chris at Airwindows - I've just simplified the build process (since we're building one plugin, not hundreds) and resolved the invalid output issue. The original repository is available [here](https://github.com/airwindows/airwindows).

## Build instructions

To build this project, you need to install CMake on your system. You also need to obtain a copy of the deprecated VST2 SDK due to Steinberg's licensing terms. This is an output of `tree` for my minimal `include` directory (thanks to the original repo):

```include/
└── vstsdk
    ├── aeffeditor.h
    ├── audioeffect.cpp
    ├── audioeffect.h
    ├── audioeffectx.cpp
    ├── audioeffectx.h
    ├── CMakeLists.txt
    ├── pluginterfaces
    │   └── vst2.x
    │       ├── aeffect.h
    │       ├── aeffectx.h
    │       └── vstfxstore.h
    └── vstplugmain.cpp
```

With this in place, on Linux, just run `build.sh --clean` and the script will handle the rest. You can grab the plugin from `./build/Capacitor2Fix.so` afterward.