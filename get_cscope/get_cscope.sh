#!/bin/bash

find ../ -name "*.c" -o -name "*.h" -o -name "*.S" -o -name "*.asm"  > find_files

g++ -o abs_add abs_add.cpp -Wall

./abs_add

cscope -bqk -i ../cscope.files

mv cscope.* ../
