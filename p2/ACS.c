#include <stdio.h>  //printf(), scanf()
#include <stdlib.h>  //malloc()
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>  //errno
#include <sys/types.h>
#include <pthread.h>
#include <sys/time.h>
#include "ACSfunc.h"

int main(int argc, char* argv[]){
	if(argc != 2){
		fprintf(stderr,"Usage:ACS customers.txt\n");
		exit(1);
	} 
	FILE *data_fp = fopen(argv[1], "r");

	if (data_fp == NULL) {
		fprintf(stderr,"unable to open %s\n", argv[1]);
		exit(1);
	}
	int cnum = 0;
	char buffer[50];
	int rc,err;
	fgets(buffer,sizeof(char)*50, data_fp);
	cnum = atoi(buffer);//customer number
	customer c[cnum];//customer array
	clerk s[4];  //clerk array
	pthread_t cth[cnum]; //customer thread array
	pthread_t sth[4]; //clerk thread array
	char *t;
	int i = 0;
	while(fgets(buffer,sizeof(char)*50, data_fp)){
		t = strtok(buffer,",:");
		int a[4];
		int j = 0;
		while (t!=NULL){
			a[j++]=atoi(t);//printf("%s\n",t);
			t = strtok(NULL, ",:");
		}
		c[i].id = a[0];
		c[i].class = a[1];
		c[i].atime = a[2];
		c[i].stime = a[3];
		i++;
	}
	
	fclose(data_fp);
	
	if (pthread_mutex_init(&lock, NULL) != 0){ //mutex initialization
        	printf("\n mutex init failed\n");
        	return 1;
    	}

	if (pthread_mutex_init(&lock1, NULL) != 0){ //mutex initialization
        	printf("\n mutex init failed\n");
        	return 1;
    	}

	/* create customer threads */
	for (i = 0; i < cnum; ++i) {
		//srand(time(NULL));
		rc = pthread_create(&cth[i], NULL, customer_function, &c[i]);
		if (rc != 0) {
			fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
			return EXIT_FAILURE;
		}
	}

	//create clerk threads
	for (i = 0; i < 4; ++i){
		s[i].id = i+1;
		s[i].status = 0;
		err = pthread_create(&sth[i], NULL, clerk_function, &s[i]);
		if (err != 0) {
			fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
			return EXIT_FAILURE;
		}
	}
	double avg = 0;
	double avge = 0;
	double avgb = 0;	
	int en = 0;
	int bn = 0;
	// block until all threads complete 
	for (i = 0; i < cnum; ++i) {
		if (c[i].class == 1){
			avgb = avgb + c[i].wtime;
			bn++;
		} 
		if (c[i].class == 0){
			avge = avge + c[i].wtime;
			en++;
		}
		avg = avg + c[i].wtime;
		pthread_join(cth[i], NULL);
		pthread_mutex_destroy(&lock);	
	}
	printf("The average waiting time for all customers in the system is: %.2f seconds. \n", avg/cnum);
	printf("The average waiting time for all business-class customers is: %.2f seconds. \n", avgb/bn);
	printf("The average waiting time for all economy-class customers is: %.2f seconds. \n", avge/en);
	for (i = 0; i < 4; ++i) {
		pthread_join(sth[i], NULL);
		pthread_mutex_destroy(&lock1);	
	}

	sleep(1);//wait for customers leaving
	return 0;
	
}
