#include <cstdio>
#include "List.h"

List<char> getList()
{
	char a[] = "hello world!\n";
	List<char> list(a,strlen(a)+1);
	list.erase(2);
	list.insert(1,'E');
	return list;
}

int main()
{
	auto list = getList();
	char a[40];
	list.to_a(a);
	printf("%s\n",a);
	return 0;
}