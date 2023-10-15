#!/bin/bash

if [ -d "build" ]; then
  rm -rf build
fi
mkdir build

cd build
cmake ..
cmake --build . --config Debug

./autodragan