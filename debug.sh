for i in "src/" "examples/"
do
  cppcheck --enable=all --std=c++11 --check-config \
  --inline-suppr \
  --suppress=missingIncludeSystem \
  --suppress=unmatchedSuppression \
  --suppress=useStlAlgorithm $i
done

cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .

ASAN_OPTIONS=detect_leaks=1
MSAN_OPTIONS=halt_on_error=1 

./autodragan

valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./autodragan
