# Capacitor2Tweak

This repository implements a simple tweak to Airwindows' Capacitor2 VST plugin. In some conditions, the original Capacitor2 plugin can produce NaN and inf floating-point outputs that are not handled gracefully by DAWs. This version of the plugin saturates the filter IIR coefficients to prevent invalid float outputs from occurring.

Most of the effort in this repository was originally done by Chris at [Airwindows](https://github.com/airwindows/airwindows) - I've  I've just simplified the build process (since we're building one plugin, not hundreds), changed the filter behavior, and ported the plugin to VST3.

## Build instructions

To build this project, you need to first pull the VST3 repository:

```
git submodule update --init --recursive
```

Once the pull is complete, you can generate the CMake configuration and run the build with:

```
# on Linux
./build.sh --clean

# on Windows
.\build.ps1 -Clean
```

On Linux, provided you have CMake and a build toolchain installed, the plugin should be available in `./build/VST3/`. On Windows, if the build doesn't work, you likely need to open the generated Visual Studio configuration in `./build/` and run the build in there.
