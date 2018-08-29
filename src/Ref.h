#ifndef GARBAGE_REF
#define GARBAGE_REF

#include <stdlib.h>
#include <cstring>
#include "HashMap.h"

template <typename T>
class Ref
{
public:

	Ref(T *ref)
	{
		ptr = ref;
		retain();
	}

	Ref (const Ref<T> &old_obj)
	{
		ptr = old_obj.ptr;
		retain();
	}

	void operator=(T *ref)
	{
		Ref<T> old(this->p());
		release();
		ptr = ref;
		retain();
	}

	void operator=(Ref<T> ref)
	{
		*this=ref.p();
	}

	T *operator->()
	{
		return ptr;
	}
	T &operator*()
	{
		return *ptr;
	}

	T &operator[](int index)
	{
		return **this[index];
	}

	T *p()
	{
		return ptr;
	}

	~Ref()
	{
		release();
	}

protected:
	T *ptr;

private:
	static HashMap allocated;

	void retain()
	{
		int *countP = (int *)allocated.read(&ptr);
		int oldCount = 0;
		if(countP != nullptr)
			oldCount =  *countP;
		allocated.write(&ptr,&++oldCount);
	}

	void release()
	{
		int *countP = (int *)allocated.read(&ptr);
		int count;
		if(countP == nullptr)
		{
			fprintf(stderr,"ERROR: Attempted to release release non-allocated block\n");
			exit(1);
		}
		count = *countP;
		if(--count == 0)
		{
			if(ptr != nullptr)
				delete(ptr);
			allocated.erase(&ptr);
		}
		else
			allocated.write(&ptr,&count);
	}
};

template <typename T>
HashMap Ref<T>::allocated(sizeof(void *),sizeof(int));

#endif