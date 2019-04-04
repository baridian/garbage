#ifndef GARBAGE_REF
#define GARBAGE_REF

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "hashTable.h"

template<typename T>
class Ref
{
public:

	/*
	 * constructs a new reference to an instance of T on the heap. Adds one reference to the R. count for that address
	 */
	Ref(T *ref)
	{
		ptr = ref;
		retain();
	}

	Ref()
	{
		ptr = nullptr;
		retain();
	}

	/*
	 * copy constructor. Adds one reference to R. count for the address referenced by old_obj
	 */
	Ref(const Ref<T> &old_obj)
	{
		ptr = old_obj.ptr;
		retain();
	}

	/*
	 * assignment operator. removes one reference to what it previously pointed to, changes pointer, and adds one
	 * reference to what it now points to. old_obj used for safety; ensures nothing is freed till it goes out of scope.
	 */
	void operator=(T *ref)
	{
		Ref<T> old(*this);
		release();
		ptr = ref;
		retain();
	}

	/*
	 * wrapper to other assignment operator
	 */
	void operator=(Ref<T> ref)
	{
		*this = ref.p();
	}

	/*
	 * dereference operator
	 */
	T *operator->()
	{
		return ptr;
	}

	/*
	 * second dereference operator
	 */
	T &operator*()
	{
		return *ptr;
	}

	/*
	 * dereference offset operator
	 */
	T &operator[](int index)
	{
		return *(ptr + index);
	}

	/*
	 * get pointer
	 */
	T *p()
	{
		return ptr;
	}
	
	operator T *() const
	{
		return ptr;
	}

	/*
	 * lower number of references to what it points to by one when object is destroyed
	 */
	~Ref()
	{
		release();
	}

	/*
	 * calls dump on the hash table, a debugging function
	 */
	void dump()
	{
		allocated->dump();
	}

protected:
	T *ptr;

private:

	/*
	 * allocated contains a hash map of all referenced pointers and the count to each
	 */
	static HashTable<void *, int> *allocated;

	/*
	 * adds one reference to value pointed to
	 */
	void retain()
	{
		allocated->contains(ptr) ? (*allocated)[ptr]++ : (*allocated)[ptr] = 1;
	}

	/*
	 * removes one reference to value pointed to; deletes it if reference count drops to 0
	 */
	void release()
	{
		if (!allocated->contains(ptr))
		{
			fprintf(stderr, "ERROR: attempted to free untracked memory.\n");
			printf("ptr val = %p\n", ptr);
			allocated->dump();
			exit(1);
		}
		if (--(*allocated)[ptr] == 0)
		{
			if (ptr != nullptr)
				delete[] ptr;
			allocated->erase(ptr);
		}
	}
};

template<typename T>
HashTable<void *, int> *Ref<T>::allocated = HashTable<void *, int>::newStaticTable();

#endif