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
int main(){

	//pid_t pid;   //The ID of child process
	int status;
	Pidval *pidList = NULL;
	char *input =(char *)malloc(sizeof(char));
	char *prompt = "PMan:> ";
	input = readline(prompt);
	char *in = (char *)malloc(sizeof(char));
	char *item[2] = {NULL,NULL}; 
	in = strtok(input, " ");
	int i = 0;
	while(in != NULL){
		item[i++] = in;
		in = strtok(NULL, " ");	
	}
	while(1){
		if(strcmp(item[0],"bg")==0 && item[1] != NULL){
		 	pidList=bgfile(pidList, item[1]);
		} else if(strncmp(item[0],"bglist",6) == 0){
			if (pidList == NULL){
				printf("Total background job: 0\n");
			} else {
				bglist(pidList);
			}
		} else if(strncmp(item[0],"bgkill",6) == 0){
			if (item[1] == NULL){
				printf("Please enter the pid\n");
			} else {
				int pid = atoi(item[1]);
				if (lookup(pidList, pid)){
					kill(pid,SIGTERM);
					int retVal;
					retVal = waitpid(pid, &status, WNOHANG | WUNTRACED | WCONTINUED);
					if (retVal == -1) {
						perror("Fail at waitpid");
						exit(EXIT_FAILURE);
					}
					if (WIFSIGNALED(status)) {
						printf("killed by signal %d\n", WTERMSIG(status));
						pidList = bgkill(pidList, pid);
					}
					printf("Process %d was killed\n", pid);
					//pidList=bgkill(pidList, pid);
				} else {printf("Error: Process %d does not exist.\n", pid);}
			}	
		} else if(strncmp(item[0],"bgstop",6) == 0){
			if (item[1] == NULL){
				printf("Please enter the pid\n");
			} else {
				int pid = atoi(item[1]);
				if (lookup(pidList, pid)){
					kill(pid, SIGSTOP);
					int retVal;
					retVal = waitpid(pid, &status, WNOHANG | WUNTRACED | WCONTINUED);
					if (retVal == -1) {
						perror("Fail at waitpid");
						exit(EXIT_FAILURE);
					}
					if (WIFSTOPPED(status)) {
						printf("stopped by signal %d\n", WSTOPSIG(status));
					}
					printf("Process %d was exits\n", pid);
				} else {printf("Error: Process %d does not exist.\n", pid);}
			}
		} else if(strncmp(item[0],"bgstart",6) == 0){
			if (item[1] == NULL){
				printf("Please enter the pid\n");
			} else {
				int pid = atoi(item[1]);
				if (lookup(pidList, pid)){
					kill(pid, SIGCONT);
					int retVal;
					retVal = waitpid(pid, &status, WNOHANG | WUNTRACED | WCONTINUED);
					if (retVal == -1) {
						perror("Fail at waitpid");
						exit(EXIT_FAILURE);
					}
					if (WIFCONTINUED(status)) {
						printf("continued\n");
					}
					printf("Process %d was continued\n", pid);
				} else {printf("Error: Process %d does not exist.\n", pid);}
			}
		} else if(strncmp(item[0],"pstat",5) == 0){
			if (item[1] == NULL){
				printf("Please enter the pid\n");
			} else {
				int pid = atoi(item[1]);
				if (lookup(pidList, pid)){pstat(pidList, item[1]);}
				else {printf("Error: Process %d does not exist.\n", pid);}
			}
		} else {
			printf("PMan:> %s:  Command not found\n",item[0]);
		}

		
		input = readline(prompt);
		item[0] = NULL;
		item[1] = NULL; 
		in = strtok(input, " ");
		i = 0;
		while(in != NULL){
			item[i++] = in;
			in = strtok(NULL, " ");	
		}
	}

	return 1;
}
