#ifndef _PMAMFUNC_H_
#define _PMANFUNC_H_

#include <stdio.h>

typedef struct Pidval Pidval;
struct Pidval {
        int pidnum;
	char *name;
        Pidval *next;
};


void *emalloc(size_t n);
Pidval *newitem(int pidnum, char *name);
Pidval *addend(Pidval *listp, Pidval *newp);
Pidval *bgfile(Pidval *pidList, char *item);
void bglist(Pidval *pidList);
Pidval *bgkill(Pidval *listp, int pid);
int lookup(Pidval *listp, int pid);
void pstat(Pidval *listp, char *pid);
#endif

