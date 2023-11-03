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
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .

ASAN_OPTIONS=detect_leaks=1
MSAN_OPTIONS=halt_on_error=1 

valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./autodragan

cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

./autodragan