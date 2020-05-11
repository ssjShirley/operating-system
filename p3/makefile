.phony all:
all: sfs

sfs:
	gcc -Wall a3.c diskinfo.c -o diskinfo
	gcc -Wall a3.c disklist.c -o disklist
	gcc -Wall a3.c diskget.c -o diskget
	gcc -Wall a3.c diskput.c -o diskput

.PHONY clean:
clean:
	-rm -rf *.o *.exe


