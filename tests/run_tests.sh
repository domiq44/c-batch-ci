#!/usr/bin/env bash
set -euo pipefail

echo "[TEST] Compilation..."
make

echo "[TEST] 1 : appel sans mode"
if ./c-batch-ci data/input.txt 2>/dev/null; then
  echo "[ECHEC] Le programme aurait dû échouer"
  exit 1
fi

echo "[TEST] 2 : mode inconnu"
if ./c-batch-ci data/input.txt unknown 2>/dev/null; then
  echo "[ECHEC] Le programme aurait dû échouer"
  exit 1
fi

echo "[TEST] 3 : mode count"
echo -e "a\nb\nc" > test_input.txt
OUT=$(./c-batch-ci test_input.txt count)
[[ "$OUT" == *"Nombre de lignes : 3"* ]]

echo "[TEST] 4 : mode words"
echo "un deux trois" > test_input.txt
OUT=$(./c-batch-ci test_input.txt words)
[[ "$OUT" == *"Nombre de mots : 3"* ]]

echo "[TEST] 5 : mode upper"
echo -e "abc\nDeF" > test_input.txt
OUT=$(./c-batch-ci test_input.txt upper)
[[ "$OUT" == *"ABC"* ]]
[[ "$OUT" == *"DEF"* ]]

echo "[TEST] 6 : mode grep"
echo -e "ligne1\nerror: pb\nligne3" > test_input.txt
OUT=$(./c-batch-ci test_input.txt grep error)
[[ "$OUT" == *"error: pb"* ]]

echo "[TEST] Tous les tests sont OK"
