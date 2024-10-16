.PHONY: setup test

CC=gcc
CFLAGS=-O3 -std=c99
TARGETS=GF.o GFPoly.o
TESTFILES=GF.o GFPoly.o

setup: $(TARGETS)
	@echo $(shell pwd)
test: test_exc
	./test_exc
test_exc: test.o $(TESTFILES)
	$(CC) $^ -o $@ $(CFLAGS)
%.o: %.c
	$(CC) $< -c -o $@ $(CFLAGS)
