#!/bin/bash

mkdir -p ./build/shaders
glslc -fshader-stage=vertex src/game/shaders/vertex.glsl -o build/shaders/vert.spv
glslc -fshader-stage=fragment src/game/shaders/fragment.glsl -o build/shaders/frag.spv
echo "built shaders"
