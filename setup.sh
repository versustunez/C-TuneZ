#!/usr/bin/sh
mkdir Build
cd Build
emcmake cmake -DCMAKE_BUILD_TYPE=Release -GNinja ..
ninja -j$(nproc)