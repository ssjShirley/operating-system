CC=gcc
CFLAGS=-c
LDFLAGS=

all: ACS

ACS: ACS.o ACSfunc.o
	$(CC) $(LDFLAGS) ACS.o ACSfunc.o -pthread -o ACS

ACS.o: ACS.c ACSfunc.h
	$(CC) $(CFLAGS) ACS.c

ACSfunc.o: ACSfunc.c ACSfunc.h
	$(CC) $(CFLAGS) ACSfunc.c
clean:
	-rm -rf *.o *.exe

