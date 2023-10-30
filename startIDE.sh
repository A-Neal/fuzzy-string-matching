#!/bin/bash

# Open terminal windows in specific arrangement
i3-msg split h
uxterm &
sleep 0.3
i3-msg split v
uxterm &

# Files to open in Vim
files=".gitignore \
	Makefile \
	src/testmain.c \
	src/fuzzymatching.c \
	include/fuzzymatching.h
	"

# What to do upon opening Vim
vim $files -c ":tab all"

