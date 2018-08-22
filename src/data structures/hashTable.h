#ifndef CRCT_HASHTABLE_H
#define CRCT_HASHTABLE_H

#define STRING_SIZE (-1)

typedef struct hashNode
{
	void *key;
	void *data;
	unsigned isOccupied : 1;
} hashNode;

typedef struct hashTable
{
	int keySize;
	int valueSize;
	int allocated;
	int indexesUsed;
	hashNode *table;
} hashTable;

static unsigned hashString(hashTable,void *);

static unsigned hash(hashTable, void *);

hashTable newHashTable(int keySize, int valueSize);

static int memcmpr(void *, void *, int);

void *readHash(hashTable, void *key);

void writeHash(hashTable *, void *key, void *value);

void eraseHashNode(hashTable *, void *key);

int contains(hashTable, void *key);

void freeHashTable(hashTable);

#endif //CRCT_HASHTABLE_H
