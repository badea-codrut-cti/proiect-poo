set -e

cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

cd .. 

cppcheck --enable=all \
  --inline-suppr \
  --inline-suppr \
  --project="./build/compile_commands.json" \
  -i "build" --suppress='*:"build"/*' \
  -i "src/topologie" --suppress='*:"src/topologie"/*' \
  --suppress=missingIncludeSystem \
  --suppress=unmatchedSuppression \
  --suppress=preprocessorErrorDirective \
  --suppress=useStlAlgorithm \
  --suppress=assertWithSideEffect

cd build

cmake --build . -j16

ASAN_OPTIONS=detect_leaks=1
MSAN_OPTIONS=halt_on_error=1 

./autodragan

valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./autodragan
