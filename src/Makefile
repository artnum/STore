CC=gcc
CFLAGS=-I../include/ -Wall
LDFLAGS=-licuuc
BUILDDIR=../build/
RM=rm

all: $(BUILDDIR)ustr.o $(BUILDDIR)mem.o $(BUILDDIR)mtrack.o 

clean:
	$(RM) $(BUILDDIR)*.o

$(BUILDDIR)ustr.o: ustr.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ -c $<

$(BUILDDIR)mem.o: mem.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ -c $<

$(BUILDDIR)mtrack.o: mtrack.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ -c $<
