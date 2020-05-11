#include <stdio.h>  //printf(), scanf()
#include <unistd.h>  //fork(), execvp()
#include <stdlib.h>  //malloc()
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>  //pid_t
#include <sys/wait.h>  //waitpid()
#include <signal.h>  //kill()
#include <errno.h>  //errno
#include "PManfunc.h"
#define ONE_MB (1024 * 1024)

void *emalloc(size_t n){ //slight detour
	void *p;

	p = malloc(n);
	if(p == NULL){
		fprintf(stderr, "malloc failed");
		exit(1);
	}
	return p;
}

Pidval *newitem(int pidnum, char *name){ //constructing an item
	Pidval *newp;

	newp = (Pidval *)emalloc(sizeof(Pidval));
	newp -> pidnum = pidnum;
	newp -> name = name;
	newp -> next = NULL;
	return newp;
}

Pidval *addend(Pidval *listp, Pidval *newp){
	Pidval *p;

	if (listp == NULL){
		return newp;
	}
	for (p = listp; p->next != NULL; p = p->next)
		;
	p->next = newp;
	return listp;
}

Pidval *bgfile(Pidval *pidList, char *item){
	pid_t pid;
	int status;
	pid = fork();
	int exe = 1;		
	if(pid == 0){
		//Child process
		char *argv_execvp[] = {item,NULL};
		char f[20] = "./";
		strcat(f, item);
		int e = execvp(f, argv_execvp);
		if (e < 0){
			printf("Error: execution of %s failed\n", f);
			exe = 0;
			exit(-1);
		}
	}
	else if(pid > 0){
		//parent process
		Pidval *newpid = newitem(pid, item);
		pidList = addend(pidList, newpid);
		int opts = WNOHANG | WUNTRACED | WCONTINUED;
		//return(pidList);
		int retVal;
		retVal = waitpid(pid, &status, opts);
		if (retVal == -1) {
			perror("Fail at waitpid");
			exit(EXIT_FAILURE);
		}
		if(exe == 0){
			pidList = bgkill(pidList, pid);
			exe = 1;
		}
		return(pidList);
		// Macros below can be found by "$ man 2 waitpid"
		if (WIFEXITED(status)) {
			printf("Normal, status code=%d\n", WEXITSTATUS(status));
		// Display the status code of child process
		} else if (WIFSIGNALED(status)) {
			printf("killed by signal %d\n", WTERMSIG(status));
		//	pidList = bgkill(pidList, pid);
		} else if (WIFSTOPPED(status)) {
			printf("stopped by signal %d\n", WSTOPSIG(status));
		} else if (WIFCONTINUED(status)) {
			printf("continued\n");
		}
		//return(pidList);
		//sleep(10);
	} else {  // Fail to create new process
		perror("\nFail to create a new process.\n");
	}
	return(pidList);
//item[i] = in;
}


void bglist(Pidval *pidList){
	int i = 1;
	Pidval *p;
	//printf("%d:%s\n", pidList->pidnum, pidList->name);
	for(p = pidList; p->next != NULL; p = p->next){
		printf("%d:%s\n", p->pidnum, p->name);
		i++;
	}
	printf("%d:%s\n", p->pidnum, p->name);
	printf("Total background job: %d\n", i);

}

Pidval *bgkill(Pidval *listp, int pid){
	//kill(pid, SIGTERM);
	Pidval *curr, *prev;

	prev = NULL;
	for(curr = listp; curr !=NULL; curr = curr->next){
		if(pid == curr->pidnum){
			
			if(prev == NULL){
				listp = curr->next;
			} else {
				prev->next = curr->next;
			}
			free(curr);
			return listp;
		}
		prev = curr;
	}
	fprintf(stderr, "bgkill: %d not in list", pid);
	exit(1);
}

int lookup(Pidval *listp, int pid){
	for ( ;listp != NULL; listp = listp->next){
		if (pid == listp->pidnum){
			return(1);
		}
	}
	return (0);
}

void pstat(Pidval *listp, char *pid){
	char *p = "/proc/";
	char str[50];
	strcpy(str, p);
	strcat(str, pid);
	strcat(str, "/stat");
	FILE *fp = fopen(str,"r");
	if (fp == NULL){
		perror("Error: cannot open file");
		exit(0);
	}
	int pidn;
	char comm[50];
	char state;
	int ppid, pgrp, session, tty_nr, tpgid;
	unsigned int flags;
	float utime, stime;
	long unsigned int minflt, cminflt, majflt, cmajflt, vsize;
	long int cutime, cstime, priority, nice, num_threads, itrealvalue, rss;
	long long unsigned int starttime;
	if(fp){
		fscanf(fp, "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %f %f %ld %ld %ld %ld %ld %ld %llu %lu %ld",&pidn, comm, &state, &ppid, &pgrp, &session, &tty_nr, &tpgid, &flags, &minflt, &cminflt, &majflt, &cmajflt, &utime, &stime, &cutime, &cstime, &priority, &nice, &num_threads, &itrealvalue, &starttime, &vsize, &rss);
	}
	fclose(fp);
	printf("comm: %s\n",comm);
	printf("state: %c\n", state);
	printf("utime: %f\n", utime*sysconf(_SC_CLK_TCK));
	printf("stime: %f\n", stime*sysconf(_SC_CLK_TCK));
	printf("rss: %ld\n", rss);

	char strs[50];
	strcpy(strs, p);
	strcat(strs, pid);
	strcat(strs, "/status");
	fp = fopen(strs,"r");
	int num=0;
	char buffer[100], shengjie[50][100];
	while(fgets(buffer,150,fp)){
		strcpy(shengjie[num],buffer);
		num++;
	}

	printf("%s",shengjie[num-2]);
	printf("%s",shengjie[num-1]);
}


