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

int qhead[2]={0,0};
int qtail[2]={-1,-1};
int counter = 0; // a counter
int sid;
pthread_cond_t convar = PTHREAD_COND_INITIALIZER; //convar initialization
pthread_cond_t convar1 = PTHREAD_COND_INITIALIZER; //convar initialization
pthread_cond_t convar2 = PTHREAD_COND_INITIALIZER; //convar initialization

int gettimeofday(struct timeval*tv, struct timezone *tz);

/* thread function */
void *customer_function(void *ptr){
	customer * cPtr = (customer *)ptr;
	usleep(cPtr->atime*100000);
	printf("A customer arrives: customer ID %2d.\n", cPtr->id);
	qtail[cPtr->class]++;
	cPtr->pos = qtail[cPtr->class];
	printf("A customer enters a queue: the queue ID %1d, and length of the queue %2d. \n", cPtr->class, qtail[cPtr->class]-qhead[cPtr->class]+1);
	struct timeval tv_begin,tv_end;
	gettimeofday(&tv_begin,NULL);
	//customer is not the first, wait
	pthread_mutex_lock(&lock); //lock critical section
	pthread_cond_signal(&convar2);
	pthread_cond_wait(&convar, &lock);
	if (cPtr->class == 1){
		while (counter>=4 || cPtr->pos != qhead[cPtr->class]){
			pthread_cond_wait(&convar, &lock);
		}
	}else{
		while(counter>=4 || qtail[1] >= qhead[1] || cPtr->pos != qhead[cPtr->class]){
			pthread_cond_wait(&convar, &lock);
		}
	}
	int s = sid;
	counter++;
	qhead[cPtr->class]++;
	gettimeofday(&tv_end,NULL);
	cPtr->wtime =tv_end.tv_sec-tv_begin.tv_sec;
	cPtr->ttime = cPtr->wtime + (double)cPtr->atime/10;//printf("wait time = %2f", cPtr->ttime);
	pthread_mutex_unlock(&lock); //unlock critical section
	printf("A clerk starts serving a customer: start time %.2f, the customer ID %1d.the clerk id is %d \n",cPtr->ttime,cPtr->id, s);
	usleep(cPtr->stime*100000);
	printf("A clerk finishes serving a customer: end time %.2f, the customer ID %1d.the clerk id is %d \n",cPtr->stime/10 + cPtr->ttime,cPtr->id,s);
	sid = s;
	counter--;
	pthread_cond_signal(&convar1);
	pthread_exit(NULL);
	return NULL;
}

int cus_in_queue(){
	if (qtail[0] >= qhead[0]){
		return 0;
	} else if (qtail[1] >= qhead[1]){
		return 1;
	} else {
		return -1;
	}
}

void *clerk_function(void *ptr){
	clerk * sPtr = (clerk *)ptr;
	sPtr->status = 0;//free
	while(cus_in_queue()==-1) ;
	pthread_mutex_lock(&lock1); //lock critical section
	pthread_cond_wait(&convar2,&lock1);
	sid = sPtr->id;
	pthread_cond_broadcast(&convar);
	pthread_cond_wait(&convar1,&lock1);
	pthread_mutex_unlock(&lock1); //unlock critical section
	while(cus_in_queue() != -1){
		pthread_mutex_lock(&lock1); //lock critical section
		pthread_cond_broadcast(&convar);
		pthread_cond_wait(&convar1,&lock1);
		pthread_mutex_unlock(&lock1); //unlock critical section

	}
	pthread_exit(NULL);
	return NULL;
}

