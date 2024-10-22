#!/bin/bash
while [[ $# -gt 0 ]]; do
    case $1 in
        -c|--clean)
            SHOULD_CLEAN=1;
            shift;
            ;;
        -t|--type)
            BUILD_TYPE="$2";
            shift;
            shift;
            ;;
        *)
            shift;
            ;;
    esac;
done;

if [[ $BUILD_TYPE == "debug" ]]; then
    cmake_build_type_arg="-DCMAKE_BUILD_TYPE=Debug";
elif [[ $BUILD_TYPE == "release" ]]; then
    cmake_build_type_arg="-DCMAKE_BUILD_TYPE=Release";
elif [[ -n $BUILD_TYPE ]]; then
    echo "Received build type that was not 'debug' or 'release'; exiting.";
    exit 1;
fi;

if [[ -d ./build ]]; then
    if [[ $SHOULD_CLEAN -eq 1 ]]; then
        rm -r ./build/*;
    fi;
else
    mkdir ./build/;
fi;

cd ./build/;
cmake .. "$cmake_build_type_arg";
make;
