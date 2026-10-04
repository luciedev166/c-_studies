#!/bin/bash
# Compiles your code and checks it against every test in tests/.
#   ./run_tests.sh
cd "$(dirname "$0")" || exit 1

FLAGS="-std=c++17 -Wall -g"
echo 'int main(){}' > .asan_probe.cpp
if g++ -fsanitize=address .asan_probe.cpp -o .asan_probe 2>/dev/null; then FLAGS="$FLAGS -fsanitize=address"; fi
rm -f .asan_probe.cpp .asan_probe

g++ $FLAGS -o test main.cpp || { echo "COMPILE ERROR"; exit 1; }
g++ -std=c++17 -o checker checker.cpp || exit 1

mkdir -p out
pass=0; total=0
for infile in tests/*.in; do
    name=$(basename "$infile" .in)
    total=$((total + 1))
    ./test < "$infile" > "out/$name.txt" 2> "out/$name.err"
    code=$?
    printf "test %s: " "$name"
    if [ $code -ne 0 ]; then
        echo "CRASHED (exit code $code). See out/$name.err"
        continue
    fi
    result=$(./checker "out/$name.txt" "tests/$name.out")
    echo "$result"
    case "$result" in PASS*) pass=$((pass + 1));; esac
done
echo
echo "$pass/$total tests passed"
[ $pass -eq $total ]
