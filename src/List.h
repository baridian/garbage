#ifndef GARBAGE_LIST
#define GARBAGE_LIST

#include "Ref.h"
#include "ListNode.h"

template <typename T>
class List
{
public:
	List(T array[], int length) : head(nullptr)
	{
		if(length > 0)
			head = Ref(new ListNode<T>(nullptr,new T));
		Ref<ListNode<T>> current(!head);
		for(int i=0;i<length;i++)
		{
			current->next = Ref(new ListNode<T>(nullptr, new T));
			current->data = array[i];
			current = current->next;
		}
	}


	T read(int index)
	{

	}
	void write(int,T);
	void insert(int,T);
	void erase(int,T);
	int length()
	{
		Ref<ListNode<T>> current(!head);
		if(!head == nullptr)
			return 0;
		int i = 1;
		for(;!current->next;current = current->next,i++);
		return i;
	}

private:
	Ref<ListNode<T>> head;

};
#endif