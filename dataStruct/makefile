.PHONY: setup

CC=gcc
CFLAGS=-O3 -std=c99
TARGETS=array.o dsforest.o

setup: $(TARGETS)
	echo setup
%.o: %.c
	$(CC) $< -c -o $@ $(CFLAGS)
