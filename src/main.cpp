#include <cstdio>
#include "List.h"

int main()
{
	char string[] = "test";
	List<char> list(string,strlen(string) + 1);
	list.insert(1,'a');
	list.to_a(string);
	printf("%s\n",string);
	return 0;
}