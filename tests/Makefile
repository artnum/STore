LDFLAGS=-licuuc -lcheck_pic -pthread -lrt -lm -lsubunit
CFLAGS=-I../include/ -Wall -pthread -ggdb
CHEKC=/usr/lib/x86_64-linux-gnu/libcheck.a
CC=gcc
RM=rm


all: tests
	./tests

tests: mem.o test_mem.o tests.c
	$(CC) $^ $(CFLAGS) $(LDFLAGS) /usr/lib/x86_64-linux-gnu/libcheck.a -o $@

test_mem.o: mem.c
	$(CC) $(CFLAGS) -c $< -o $@

mem.o: ../src/mem.c
	$(CC) $(CFLAGS) -c $< -o $@

mem-val: ../src/mem.c mem-val.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
	valgrind --leak-check=full  ./mem-val
