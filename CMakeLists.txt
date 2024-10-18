cmake_minimum_required(VERSION 3.9)
project(airwindows_ports)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-D__cdecl=)

add_library(Capacitor2Fix SHARED
    ./src/Capacitor2Fix.cpp
    ./src/Capacitor2FixProc.cpp
    include/vstsdk/vstplugmain.cpp
    include/vstsdk/audioeffect.cpp
    include/vstsdk/audioeffectx.cpp)
target_include_directories(Capacitor2Fix
    PUBLIC "./src"
    PUBLIC "./include/vstsdk")

set_target_properties(Capacitor2Fix PROPERTIES PREFIX "")
