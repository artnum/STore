LDFLAGS=-licuuc -lcheck_pic -pthread -lrt -lm -lsubunit
CFLAGS=-I../include/ -Wall -pthread -ggdb -DNDEBUG
CHEKC=/usr/lib/x86_64-linux-gnu/libcheck.a
CC=gcc
RM=rm


all: tests
	./tests

clean:
	$(RM) -f *.o
	$(RM) -f mem-val
	$(RM) -f mtrack-val
	$(RM) -f tests

tests: mem.o test_mem.o tests.c test_ustr.o ustr.o test_mtrack.o mtrack.o
	$(CC) $^ $(CFLAGS) $(LDFLAGS) /usr/lib/x86_64-linux-gnu/libcheck.a -o $@

test_mem.o: mem.c
	$(CC) $(CFLAGS) -c $< -o $@

mem.o: ../src/mem.c
	$(CC) $(CFLAGS) -c $< -o $@

valgrind: ../src/mem.c mem-val.c ../src/mtrack.c mtrack-val.c
	$(CC) $(CFLAGS) $(LDFLAGS) ../src/mem.c mem-val.c -o mem-val
	$(CC) $(CFLAGS) $(LDFLAGS) ../src/mtrack.c mtrack-val.c -o mtrack-val
	valgrind --leak-check=full  ./mem-val
	valgrind --leak-check=full  ./mtrack-val

test_ustr.o: ustr.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

ustr.o: ../src/ustr.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

test_mtrack.o: mtrack.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

mtrack.o: ../src/mtrack.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@
