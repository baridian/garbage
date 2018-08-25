#ifndef GARBAGE_LISTNODE
#define GARBAGE_LISTNODE

#include "Ref.h"


template <typename T>
class ListNode
{
public:
	ListNode(Ref<ListNode<T>> next, T data) : next(next)
	{
		this->data = data;
	}

	ListNode(Ref<ListNode<T>> next) : next(next)
	{

	}

	ListNode(ListNode<T> *next, T data) : next(next)
	{
		this->data = data;
	}

	Ref<ListNode<T>> next;
	T data;
};

#endif