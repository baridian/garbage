#include <cstdio>
#include "List.h"

int main()
{
	int a[] = {1,2,3};
	List<int> list(a,3);
	list.erase(1);
	list.insert(1,5);
	list.to_a(a);
	return 0;
}