all: testrun

testrun: src/testmain.c src/fuzzymatching.c include/fuzzymatching.h
	gcc -Iinclude/ -Wall src/testmain.c src/fuzzymatching.c -o $@
