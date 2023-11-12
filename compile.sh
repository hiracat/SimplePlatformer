#!/bin/bash

cmake -B build -S .
pushd build
make
popd
./build/SimplePlatformer

