#!/bin/sh
echo "Compiling Q..."
../../q increment.q increment.asm
echo "Assembling..."
nasm -f elf -o increment.obj increment.asm
echo "Compiling..."
g++ -m32 increment.cpp increment.obj