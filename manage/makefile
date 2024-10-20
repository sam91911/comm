.PHONY: setup test

CC=gcc
CFLAGS=-O3 -std=c99
TARGETS=account.o
TESTFILES=account.o
LIBS = -lsqlite3 -lcrypto -lssl

setup: $(TARGETS)
	@echo $(shell pwd)
test: test_exc
	./test_exc
test_exc: test.o $(TESTFILES)
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS)
%.o: %.c
	$(CC) $< -c -o $@ $(CFLAGS)
