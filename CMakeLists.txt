cmake_minimum_required(VERSION 3.15.0)
project(airwindows_capacitor2_fix VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-D__cdecl=)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    message(STATUS "Defaulting to release type 'Release'")
    set(CMAKE_BUILD_TYPE Release)
endif()

option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES "Enable VST 3 Plug-in Examples" OFF)
option(SMTG_ENABLE_VST3_HOSTING_EXAMPLES "Enable VST 3 Hosting Examples" OFF)
option(SMTG_ENABLE_VSTGUI_SUPPORT "Enable VSTGUI Support" OFF)

set(vst3sdk_SOURCE_DIR "${PROJECT_SOURCE_DIR}/include/vst3sdk/")
add_subdirectory("${vst3sdk_SOURCE_DIR}")

smtg_enable_vst3_sdk()

smtg_add_vst3plugin(
    Capacitor2Fix
    ${PROJECT_SOURCE_DIR}/src/Entry.cpp
    ${PROJECT_SOURCE_DIR}/src/Capacitor2FixController.hpp
    ${PROJECT_SOURCE_DIR}/src/Capacitor2FixController.cpp
    ${PROJECT_SOURCE_DIR}/src/Capacitor2FixProcessor.hpp
    ${PROJECT_SOURCE_DIR}/src/Capacitor2FixProcessor.cpp
    ${PROJECT_SOURCE_DIR}/src/LookupTable.hpp
    ${PROJECT_SOURCE_DIR}/src/LookupTable.cpp)

target_link_libraries(Capacitor2Fix PRIVATE sdk)

smtg_target_configure_version_file(Capacitor2Fix)

set_target_properties(Capacitor2Fix PROPERTIES PREFIX "")
