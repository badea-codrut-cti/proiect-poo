cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

cd .. 

for i in "src/" "examples/"
do
  cppcheck --enable=all \
  --inline-suppr \
  --inline-suppr \
  --project="./build/compile_commands.json" \
  --suppress=missingIncludeSystem \
  --suppress=unmatchedSuppression \
  --suppress=useStlAlgorithm $i
done

cd build

cmake --build .

ASAN_OPTIONS=detect_leaks=1
MSAN_OPTIONS=halt_on_error=1 

./autodragan

valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./autodragan