cmake_minimum_required(VERSION 3.9)
project(airwindows_ports)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-O2 -D__cdecl=)

include(Helpers.cmake)

add_library(Capacitor2Fix SHARED
    ./src/Capacitor2.cpp
    ./src/Capacitor2Proc.cpp
    include/vstsdk/vstplugmain.cpp
    include/vstsdk/audioeffect.cpp
    include/vstsdk/audioeffectx.cpp)
target_include_directories(Capacitor2Fix
    PUBLIC "./src"
    PUBLIC "./include/vstsdk")

set_target_properties(Capacitor2Fix PROPERTIES PREFIX "")
