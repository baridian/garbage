
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <iostream>

static const float MAX_LOAD = 0.7; //maximum load before scaling up
static const int START_SIZE = 5; //smallest hash size

typedef enum bucketState //state of buckets in hash table
{
	occupied, empty, deleted
} bucketState;

template<typename keyT, typename valueT>
struct bucket //bucket structure for use in hash table, could not use typedef because of templates
{
	bucketState state;
	keyT key;
	valueT value;
};


template<typename keyT, typename valueT>
class HashTable
{
public:

	/*
	 * generates and zeroes out new hash table
	 */
	HashTable()
	{
		reset();
	}

	void reset()
	{
		table = new bucket<keyT, valueT>[START_SIZE];
		tableSize = START_SIZE;
		int i = 0;
		for (; i < START_SIZE; i++)
			table[i].state = empty;
	}

	/*
	 * creates a new static hash table for the hash table template
	 * //TODO: create more flexible solution -- hash of hashes?
	 */
	static HashTable *newStaticTable()
	{
		static bool tableExists = false;
		static HashTable<keyT, valueT> *hashTable;
		if (!tableExists)
		{
			hashTable = new HashTable();
		}
		tableExists = true;
		return hashTable;
	}

	~HashTable()
	{
		delete[] table;
	}

	/*
	 * generates a starting index for a key
	 */
	unsigned hash(keyT key)
	{
		unsigned hashVal = 0;
		int i = 0;
		int multiplicand = 31;
		for (; i < sizeof(keyT); i++)
		{
			hashVal += *((char *) (&key) + i) * multiplicand;
			multiplicand *= multiplicand;
		}
		return hashVal % tableSize;
	}

	/*
	 * accessor operator. Returns reference to value, can be used for reading and writing
	 */
	valueT &operator[](keyT key)
	{
		auto index = hash(key);
		auto start = index;
		while (table[index].state != empty)
		{
			if (table[index].state == occupied)
			{
				if (table[index].key == key)
					return table[index].value;
			}

			index = ++index % tableSize;
			if (index == start)
			{
				fprintf(stderr, "ERROR: hash table full. This should never happen.\n");
				exit(1);
			}
		}

		table[index].state = occupied;
		table[index].key = key;
		rescale();

		index = hash(key); //need to repeat process to get return value since it could have moved in rescale
		start = index;
		while (table[index].state != empty)
		{
			if (table[index].state == occupied)
			{
				if (table[index].key == key)
					break;
			}
			index = ++index % tableSize;
			if (index == start)
			{
				fprintf(stderr, "ERROR: hash table full. This should never happen.\n");
				exit(1);
			}
		}
		if (table[index].key != key)
		{
			fprintf(stderr, "value was not rehashed. This should never happen.\n");
			exit(1);
		}
		return table[index].value;
	}

	/*
	 * erases 1 bucket from the hash table
	 */
	void erase(keyT key)
	{
		auto index = hash(key);
		auto start = index;
		while (table[index].state != empty)
		{
			if (table[index].state == occupied)
			{
				if (table[index].key == key)
				{
					table[index].state = deleted;
					rescale();
					return;
				}
			}
			index = ++index % tableSize;
			if (index == start)
			{
				fprintf(stderr, "ERROR: hash table full. This should never happen.\n");
				exit(1);
			}
		}
	}

	/*
	 * returns true if key is found in table, false otherwise
	 */
	bool contains(keyT key)
	{
		unsigned index = hash(key);
		auto start = index;
		while (table[index].state != empty)
		{
			if (table[index].state == occupied)
			{
				if (table[index].key == key)
					return true;
			}
			index = ++index % tableSize;
			if (index == start)
			{
				fprintf(stderr, "ERROR: hash table full. This should never happen.\n");
				exit(1);
			}
		}
		return false;
	}


	/*
	 * dumps hash table data to console. Debugging method
	 */
	void dump()
	{
		printf("table %p:\n", this);
		for (int i = 0; i < tableSize; i++)
		{
			printf("bucket %d: ", i);
			if (table[i].state == empty)
				printf("empty\n");
			else if (table[i].state == deleted)
				printf("deleted\n");
			else
				std::cout << table[i].key << ", " << table[i].value << std::endl;
		}
		putchar('\n');
	}

	bool isEmpty()
	{
		int i = 0;
		for(; i < tableSize; i++)
		{
			if(table[i].state == occupied)
				return false;
		}
		return true;
	}

private:

	bucket<keyT, valueT> *table;
	int tableSize;

	/*
	 * determines if table must be rescaled and rescales it
	 * TODO: unclean code. performs 2 functions. Refactor!
	 */
	void rescale()
	{
		int i = 0;
		int numOccupied = 0;
		int oldSize = tableSize;
		int numEmpty = 0;
		int index;
		bucket<keyT, valueT> *newTable;

		for (; i < tableSize; i++)//count number of occupied and empty slots
		{
			if (table[i].state == occupied)
				numOccupied++;
			if (table[i].state == empty)
				numEmpty++;
		}

		if (numOccupied >= tableSize * MAX_LOAD ||
			(numOccupied + 1 < tableSize * MAX_LOAD / 2 && tableSize > START_SIZE) ||
			numEmpty == 0)
		{
			if (numOccupied > tableSize * MAX_LOAD)//abjust table size
			{
				tableSize *= 2;
				tableSize += 1;
			}
			else if (numOccupied < tableSize * MAX_LOAD / 2 && tableSize > START_SIZE)
			{
				tableSize -= 1;
				tableSize /= 2;
			}
			newTable = new bucket<keyT, valueT>[tableSize];

			for (i = 0; i < tableSize; i++) //zero out new table
				newTable[i].state = empty;

			for (i = 0; i < oldSize; i++) // copy and rehash old data into new table
			{
				if (table[i].state == occupied)
				{
					index = hash(table[i].key);
					while (newTable[index].state != empty)
						index = ++index % tableSize;
					newTable[index] = table[i];
				}
			}

			delete[] table;
			table = newTable;
		}
	}
};

#endif