#!/bin/bash

echo "======================================"
echo "Z07 Language Translator - Test Suite"
echo "Author: Zaplyvanyi Daniil"
echo "Variant: 07"
echo "======================================"
echo ""

# Test 1: Linear Algorithm
echo ">>> Test 1: Linear Algorithm <<<"
./lab_4_5 test1_linear.z07 > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✅ Compilation: SUCCESS"
    echo "Input: A=15, B=5"
    echo -e "15\n5" | ./output
    echo ""
else
    echo "❌ Compilation: FAILED"
    echo ""
fi

# Test 2: Branching Algorithm
echo ">>> Test 2: Branching Algorithm <<<"
./lab_4_5 test2_branch.z07 > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✅ Compilation: SUCCESS"
    echo "Input: A=25, B=15, C=30"
    echo -e "25\n15\n30" | ./output
    echo ""
else
    echo "❌ Compilation: FAILED"
    echo ""
fi

# Test 3: Loop Algorithm
echo ">>> Test 3: Loop Algorithm <<<"
./lab_4_5 test3_loop.z07 > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✅ Compilation: SUCCESS"
    echo "Input: A=3, B=5"
    echo -e "3\n5" | ./output
    echo ""
else
    echo "❌ Compilation: FAILED"
    echo ""
fi

echo "======================================"
echo "All tests completed!"
echo "======================================"
