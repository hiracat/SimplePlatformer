#!/bin/bash

cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
pushd build
make
popd
./build/SimplePlatformer

