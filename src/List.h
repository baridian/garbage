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
		if (length > 0)
			head = Ref<ListNode<T>>(new ListNode<T>(nullptr));
		Ref<ListNode<T>> current(head);
		for (int i = 0; i < length - 1; i++)
		{
			current->next = Ref<ListNode<T>>(new ListNode<T>(nullptr));
			current->data = array[i];
			current = current->next;
		}
		current->data = array[length - 1];
	}

	T read(int index)
	{
		Ref<ListNode<T>> current(head);
		if(head.p() == nullptr)
			exit(1);
		if(index == 0)
			return *head->data;
		if(index >= 0 && index < length())
		{
			for(int i=0;i<index;i++)
				current = current->next;
			return current->data;
		}
		else
			exit(1);
	}

	void write(int index,T data)
	{
		Ref<ListNode<T>> current(head);
		if(head.p() == nullptr)
			exit(1);
		if(index==0)
			*head->data = data;
		else if(index > 0 && index < length())
		{
			for(int i=0;i<index;i++)
				current = current->next;
			current->data = data;
		}
		else
			exit(1);
	}

	void insert(int index, T data)
	{
		Ref<ListNode<T>> current(head);

		if(head.p() == nullptr)
			exit(1);
		if(index==0)
		{
			head = Ref<ListNode<T>>(current.p());
			head->data = data;
		}
		else if(index > 0 && index < length())
		{
			for(int i=1;i<index;i++)
				current = current->next;
			current->next = Ref<ListNode<T>>(new ListNode<T>(current->next.p()));
			current->next->data = data;
		}
		else
			exit(1);
	}

	void to_a(T *writeTo)
	{
		Ref<ListNode<T>> current(head);
		for(int i=0;i<length();i++)
		{
			writeTo[i] = current->data;
			current = current->next;
		}
	}

	void erase(int index)
	{
		Ref<ListNode<T>> current(head);
		if(index==0)
		{
			head = head->next;
		}
		else if(index > 0 && index < length())
		{
			for(int i=1;i<index;i++)
				current = current->next;
			current->next = current->next->next;
		}
	}

	int length()
	{
		Ref<ListNode<T>> current(head);
		if(head.p() == nullptr)
			return 0;
		int i = 1;
		for(;current->next.p();current = current->next,i++);
		return i;
	}

private:
	Ref<ListNode<T>> head;

};
#endif