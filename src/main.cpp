#include <cstdio>
#include "Ref.h"

int main()
{
	Ref<int> a(new int);
	Ref<int> b(new int);
	a = b;

	*!a = 12;
	return 0;
}