#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "a3.h"
#define timeOffset 14 //offset of creation time in directory entry
#define dateOffset 16 //offset of creation date in directory entry



void print_date_time(char * directory_entry_startPos){
	
	int time, date;
	int hours, minutes, day, month, year;
	
	time = *(unsigned short *)(directory_entry_startPos + timeOffset);
	date = *(unsigned short *)(directory_entry_startPos + dateOffset);
	
	//the year is stored as a value since 1980
	//the year is stored in the high seven bits
	year = ((date & 0xFE00) >> 9) + 1980;
	//the month is stored in the middle four bits
	month = (date & 0x1E0) >> 5;
	//the day is stored in the low five bits
	day = (date & 0x1F);
	
	printf("%d-%02d-%02d ", year, month, day);
	//the hours are stored in the high five bits
	hours = (time & 0xF800) >> 11;
	//the minutes are stored in the middle 6 bits
	minutes = (time & 0x7E0) >> 5;
	
	printf("%02d:%02d\n", hours, minutes);
	
	return ;	
}



void printDirectoryListing(char* p) {
	while (p[0] != 0x00) {
		char fileType;
		if ((p[11] & 0b00010000) == 0b00010000) {
			fileType = 'D';
		} else {
			fileType = 'F';
		}
		
		char* fileName = malloc(sizeof(char));
		int i;
		for (i = 0; i < 8; i++) {
			if (p[i] == ' ') {
				break;
			}
			fileName[i] = p[i];
		}

		char* fileExtension = malloc(sizeof(char));
		for (i = 0; i < 3; i++) {
			fileExtension[i] = p[i+8];
		}

		strcat(fileName, ".");
		strcat(fileName, fileExtension);
		
		int fileSize = getFileSize(fileName, p);

		if ((p[11] & 0b00000010) == 0 && (p[11] & 0b00001000) == 0) {
			printf("%c %10d %20s ", fileType, fileSize, fileName);
			print_date_time(p);
		}
		
		// free(fileName);
		// free(fileExtension);

		p += 32;
	}
}


void findSubdirectory(char *p){
	int q = 0;
	while (p[0] != 0x00) {
		if ((p[11] & 0b00010000) == 0b00010000) {
			char* fileName = malloc(sizeof(char));
                	int i;
               		for (i = 0; i < 8; i++) {
                        	if (p[i] == ' ') {
                                	break;
                        	}
                        	fileName[i] = p[i];
                	}
			printf("\n%s\n===========\n",fileName);
			int x = SECTOR_SIZE *(p[26] + (p[27]<<8) +12) + 64 - q*32;
			p +=x;
			printDirectoryListing(p);
		}
		p+=32;
		q++;
	}
}



int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Error: use as follows disklist <file system image>\n");
		exit(1);
	}

	// Open disk image and map memory
	int fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		printf("Error: failed to read disk image\n");
		exit(1);
	}
	struct stat buf;
	fstat(fd, &buf);
	char* p = mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED) {
		printf("Error: failed to map memory\n");
		exit(1);
	}

	// List all entries in disk image root directory
	printf("Root\n=========\n");
	printDirectoryListing(p + SECTOR_SIZE * 19);
	// List subdirectory
	findSubdirectory(p + SECTOR_SIZE * 19);
	// Clean up
	munmap(p, buf.st_size);
	close(fd);

	return 0;
}
