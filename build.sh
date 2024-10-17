#!/bin/sh

if [[ -d ./build ]]; then
    rm -r ./build/*;
else
    mkdir ./build/;
fi;

cd ./build/;
cmake ..;
make;
