#!/usr/bin/env bash
# Builds and runs every tests/TC_*.cpp on Linux/macOS.
# Usage: tests/run_tests.sh [--coverage]   (coverage uses gcov, no other tools)
set -u
cd "$(dirname "$0")/.."
OUT=build/tests; OBJ=$OUT/obj
rm -rf "$OUT"; mkdir -p "$OBJ"
FLAGS="-std=c++17 -Wall -Wextra -pedantic"
COV=0
if [ "${1:-}" = "--coverage" ]; then COV=1; FLAGS="$FLAGS -O0 --coverage"; fi

# Game logic only: main.cpp and the Windows-only controller are excluded.
OBJS=""
for f in $(ls src/*.cpp | grep -v -E 'src/(main|GameController)\.cpp'); do
  o="$OBJ/$(basename "$f" .cpp).o"
  g++ $FLAGS -c "$f" -o "$o" || exit 1
  OBJS="$OBJS $o"
done

pass=0; total=0
for t in tests/TC_*.cpp; do
  n=$(basename "$t" .cpp)
  # TC_PORT_01 asserts the Windows target (SRS portability); run by the Windows runner.
  if [ "$n" = "TC_PORT_01" ]; then echo "[SKIP] $n (Windows-only)"; continue; fi
  total=$((total+1))
  if g++ $FLAGS "$t" $OBJS -o "$OUT/$n" && "$OUT/$n"; then
    echo "[PASS] $n"; pass=$((pass+1))
  else
    echo "[FAIL] $n"
  fi
done
echo "Results: $pass / $total passed"

if [ $COV -eq 1 ]; then
  echo "--- Coverage of src/ (all tests combined) ---"
  (cd "$OBJ" && gcov -b -o . *.o 2>/dev/null | grep -A3 "^File 'src/" \
     | grep -E "^File|Lines executed|Branches executed" \
     | grep -v "^--")
fi
[ "$pass" -eq "$total" ]
