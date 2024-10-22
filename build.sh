#!/bin/bash
while [[ $# -gt 0 ]]; do
    case $1 in
        -c|--clean)
            SHOULD_CLEAN=1;
            shift;
            ;;
        *)
            shift;
            ;;
    esac;
done;

if [[ -d ./build ]]; then
    if [[ $SHOULD_CLEAN -eq 1 ]]; then
        rm -r ./build/*;
    fi;
else
    mkdir ./build/;
fi;

cd ./build/;
cmake ..;
make;
