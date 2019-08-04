CC=gcc
LDFLAGS=-licuuc
CFLAGS=-Wall

all: icu1.out bfill.out

icu1.out: icu1.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

bfill.out: bfill.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@
