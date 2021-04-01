# simple makefile for clang-tidy and compiling
#
# Liyao Zhang 
# 10/11/2020
# CSC 250 Lab 4

SOURCE = lab4.c get_args.c
EXECUTABLE = lab4
COMPILE_FLAGS = -ansi -pedantic -Wall -g

all: tidy
	gcc $(COMPILE_FLAGS) -o $(EXECUTABLE) $(SOURCE)

tidy: $(SOURCE)
	clang-tidy -checks='*' $(SOURCE) -- -std=c99

clean:
	rm -rf $(EXECUTABLE)
