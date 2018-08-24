#ifndef GARBAGE_LISTNODE
#define GARBAGE_LISTNODE

#include "Ref.h"


template <typename T>
class ListNode
{
public:
	ListNode(Ref<ListNode<T>> next, Ref<T> data) : next(next),data(data)
	{

	}

	ListNode(ListNode<T> *next, T *data) : next(next),data(data)
	{

	}

	Ref<ListNode<T>> next;
	Ref<T> data;
};

#endif