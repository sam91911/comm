.PHONY: setup test

CC=gcc
CFLAGS=-O3 -std=c99
TARGETS=array.o pool.o dsforest.o rbtree.o heap.o list.o queue.o stack.o
TESTFILES=pool.o rbtree.o

setup: $(TARGETS)
	@echo $(shell pwd)
test: test_exc
	./test_exc
test_exc: test.o $(TESTFILES)
	$(CC) $^ -o $@ $(CFLAGS)
%.o: %.c
	$(CC) $< -c -o $@ $(CFLAGS)
