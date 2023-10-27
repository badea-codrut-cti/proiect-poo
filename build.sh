#!/bin/bash

if [ -d "build" ]; then
  rm -rf build
fi
mkdir build

cd build
cmake ..
cmake --build . --config Debug

ASAN_OPTIONS=detect_leaks=1 ./autodragan

cd ..

for i in "src/" "examples/"
do
  cppcheck --enable=all --std=c++11 --check-config --suppress=missingIncludeSystem $i
done

cd build
cmake ..
cmake --build . --config Debug

MSAN_OPTIONS=halt_on_error=1 ./autodragan

cd ..
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./build/autodragan

echo "==========================================="

./build/autodragan