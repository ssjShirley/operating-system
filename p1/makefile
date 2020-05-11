CC=gcc
CFLAGS=-c
LDFLAGS=

all: PMan

PMan: PMan.o PManfunc.o
	$(CC) $(LDFLAGS) PMan.o PManfunc.o -lreadline -lhistory -o PMan

PMan.o: PMan.c PManfunc.h
	$(CC) $(CFLAGS) PMan.c

PManfunc.o: PManfunc.c PManfunc.h
	$(CC) $(CFLAGS) PManfunc.c
clean:
	-rm -rf *.o *.exe
