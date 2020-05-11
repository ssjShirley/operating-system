#ifndef _ACDFUNC_H_
#define _ACDFUNC_H_


pthread_mutex_t lock; //mutex declaration
pthread_mutex_t lock1; //mutex declaration

int gettimeofday(struct timeval*tv, struct timezone *tz);

typedef struct _customer{
	int id;
	int class; //business = 1, economy = 0
	int atime; //arrival time
	int stime; //service time
	int pos;
	double wtime;
	double ttime;
}customer;

typedef struct _clerk{
	int id;
	int status;
}clerk;


void *customer_function(void *ptr);
int cus_in_queue();
void *clerk_function(void *ptr);

#endif
