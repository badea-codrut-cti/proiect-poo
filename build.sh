#!/bin/bash

for i in "src/" "examples/"
do
  cppcheck --enable=all --std=c++11 --check-config --suppress=missingIncludeSystem $i
done

if [ -d "build" ]; then
  rm -rf build
fi
mkdir build

cd build
cmake ..
cmake --build . --config Debug

./autodragan