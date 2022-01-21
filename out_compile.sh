#/bin/bash
nasm -felf64 out.s -o out.o
ld out.o -o out -m elf_x86_64
rm out.o
