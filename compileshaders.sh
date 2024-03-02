#!/bin/bash

glslc -fshader-stage=vertex src/shaders/vertex.glsl -o build/shaders/vert.spv
glslc -fshader-stage=fragment src/shaders/fragment.glsl -o build/shaders/frag.spv
echo "built shaders"
