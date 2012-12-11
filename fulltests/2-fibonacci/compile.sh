#!/bin/sh
echo "Compiling Q..."
../../q fibonacci.q fibonacci.asm
echo "Assembling..."
nasm -f elf -o fibonacci.obj fibonacci.asm
echo "Compiling..."
g++ -m32 fibonacci.cpp fibonacci.obj