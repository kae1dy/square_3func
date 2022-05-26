all: prog

prog: main.o func.o
	gcc -o prog main.o func.o -m32
main.o: main.c
	gcc -c -o main.o main.c -m32
func.o: func.asm
	nasm -f elf32 -o func.o func.asm
clean:
	rm *.o
