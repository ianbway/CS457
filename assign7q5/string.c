// Ian Braudaway
// String class

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "string.h"

struct STRING
{
	char* value;
};

STRING *
newSTRING(char* x)
{
	STRING *p = malloc(sizeof(STRING));
	if (p == 0) { fprintf(stderr, "out of memory\n"); exit(1); }
	p->value = x;
	return p;
}

char*
getSTRING(STRING *v)
{
	return v->value;
}

char*
setSTRING(STRING *v, char* x)
{
	char* old = v->value;
	v->value = x;
	return old;
}

void
displaySTRING(FILE *fp, void *v)
{
	fprintf(fp, "%s", getSTRING(v));
}

int
compareSTRING(void *v, void *w)
{
	STRING *s1 = (STRING*)v;
	STRING *s2 = (STRING*)w;
	char* c1 = (char *)s1->value;
	char* c2 = (char *)s2->value;
	return strcmp(c1, c2);
}
