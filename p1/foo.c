#include <stdio.h>
#include <string.h>
int main(int argc, char* argv[]){
	int i = 0;
	for(i = 0; i <=1000; i++){
		if (i == 999){
			printf("Hello, World\n");
			sleep(10);
			i = 0;
		}
	}
}
