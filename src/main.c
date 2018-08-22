#include <stdlib.h>

int main()
{
	int *p = (int *)malloc(sizeof(int)); //initialization block
	int *q = (int *)malloc(sizeof(int));
	int *i = NULL;
	addRef(p);
	addRef(q);

	remRef(q);
	q = p;
	addref(q);

	remRef(p);
	p = (int *)malloc(sizeof(int));
	addRef(p);

	*q = 13;
	i = q + 1;

	remRef(p);
	p = q;
	addRef(p);

	remRef(p);
	remRef(q);
	remRef(i);
}