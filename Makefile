all: testrun

testrun: testmain.c fuzzymatching.c fuzzymatching.h
	gcc -Iinclude/ -Wall testmain.c fuzzymatching.c -o $@
