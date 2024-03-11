all: demo

demo: src/demo.c src/fuzzymatching.c include/fuzzymatching.h
	gcc -Iinclude/ -Wall src/demo.c src/fuzzymatching.c -o demo/$@.out
