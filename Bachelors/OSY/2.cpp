#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <cmath>
using namespace std;
#endif /* __PROGTEST__ */


const int BLOCKS_MAX = 32;
const int MAGIC = 68578;


struct DataBlock
{
	bool free;
	int size;
	int8_t pow2;
	int magic;
	DataBlock* next;
};


class MemAllocator
{
public:
	MemAllocator()
	{

	}

	MemAllocator(void* memPool, int memSize) : m_pool(memPool), m_size(memSize),
		m_pending(0)
	{

		for (size_t i = 0; i < BLOCKS_MAX; i++)
			blocks[i] = nullptr;
		lb = (uint8_t*)memPool;
		ub = (uint8_t*)((uint8_t*)memPool + memSize);

		int offset = 0;
		int pow = LowerPowOf2(memSize);
		while (pow > 2)
		{
			m_size = 1 << pow; // Squeeze the size to lower pow of 2
			memSize = memSize - m_size;
			DataBlock * d = InitBlock(pow, m_size, offset);
			offset += m_size;
			ReserveInLayer(d);
			pow = LowerPowOf2(memSize);
		}

	}

	//Allocates block of requested size
	void* Alloc(int size)
	{
		size += sizeof(DataBlock);
		int pow = LowerPowOf2(size) + 1;
		//Find and reserve free layer
		DataBlock* db = FreeLayer(pow, size);
		//No layer found, abort
		if (!db)
		{
			return nullptr;
		}

		db->free = false;
		m_pending++;

		return db + 1;
	}
	bool Free(void* blk)
	{
		//Ignore fragmentation for now
		DataBlock* db = (DataBlock*)((uint8_t*)blk - sizeof(DataBlock));
		if (db && db->magic == MAGIC)
		{
			db->free = true;
			Merge(db);
			m_pending--;
		}

		return db && db->magic == MAGIC;
	}
	DataBlock* GetMeAFriend(DataBlock* block)
	{
		uint8_t* add = ((uint8_t*)block + block->size);
		if (add >= lb && add <= ub)
		{
			DataBlock* r = (DataBlock*)(add);
			if (r && r->magic == MAGIC && r->free && r->size == block->size)
				return r;
		}
		add = ((uint8_t*)block - block->size - sizeof(DataBlock));
		if (add >= lb && add <= ub)
		{
			DataBlock* r = (DataBlock*)(add);
			if (r && r->magic == MAGIC && r->free && r->size == block->size)
				return r;
		}
		return NULL;
	}
	//Merges the data block with current array
	void Merge(DataBlock* block)
	{
		DataBlock* b = GetMeAFriend(block);

		while (b)
		{
			RemoveFromLayer(b);
			if (b < block)
			{
				DataBlock* tmp = b;
				b = block;
				block = tmp;
			}
			block->size *= 2;
			block->pow2++;
			b = GetMeAFriend(block);
		}

		ReserveInLayer(block);
	}


	void Flush(int* pending)
	{
		*pending = m_pending;
	}

private:
	int LowerPowOf2(int num)
	{
		int i = 0;
		while (num)
		{
			i++;
			num >>= 1;
		}
		return i - 1;
	}
	DataBlock* InitBlock(int8_t pow, int size, int offset)
	{
		DataBlock* ret = (DataBlock*)(((uint8_t*)m_pool + offset));
		ret->size = m_size;
		ret->free = true;
		ret->pow2 = pow;
		ret->magic = MAGIC;
		return ret;
	}
	void ReserveInLayer(DataBlock* db)
	{
		db->free = true;
		db->next = blocks[db->pow2];
		blocks[db->pow2] = db;
	}

	void RemoveFromLayer(DataBlock* db)
	{
		DataBlock* start = blocks[db->pow2];
		db->free = false;
		if (start == db)
		{
			blocks[db->pow2] = db->next;
			return;
		}

		while (start)
		{
			if (start->next == db)
				start->next = db->next;

			start = start->next;
		}
	}

	//Finds first free layer where we can fit our data
	DataBlock* FreeLayer(int start, int size)
	{
		while (!blocks[start])
		{
			if (++start >= BLOCKS_MAX)
				return NULL; //Not enought mem
		}

		return ModifyLayers(&start, size);
	}

	//Picks the requested layer and gives it as low mem as possible
	DataBlock* ModifyLayers(int* requestedLayer, long unsigned int requestedBlockSize)
	{
		DataBlock* db = blocks[*requestedLayer];
		RemoveFromLayer(db);

		while ((db->size) / 2 - sizeof(DataBlock) >= requestedBlockSize)
			db = Split(db);

		return db;
	}

	//Splits requested data block in half and adds its half to our array
	DataBlock* Split(DataBlock* db)
	{
		db->free = true;
		db->size /= 2;
		db->pow2--;
		db->magic = MAGIC;
		DataBlock* buddy;
		buddy = (DataBlock*)((uint8_t*)db + db->size);
		buddy->free = false;
		buddy->size = db->size;
		buddy->pow2 = db->pow2;
		buddy->magic = MAGIC;
		Merge(db);
		return buddy;
	}

	DataBlock* blocks[BLOCKS_MAX];
	DataBlock* head;
	void* m_pool;
	int m_size;
	int m_pending;
	uint8_t* lb;
	uint8_t* ub;
};

MemAllocator all;

void HeapInit(void* memPool, int size)
{
	all = MemAllocator(memPool, size);
}

void* HeapAlloc(int size)
{
	return all.Alloc(size);
}

bool HeapFree(void* blk)
{
	return all.Free(blk);
}

void HeapDone(int* pendingBlk)
{
	all.Flush(pendingBlk);
}

#ifndef __PROGTEST__
int main(void)
{
	uint8_t* p0, * p1, * p2, * p3, * p4;
	int pendingBlk;
	static uint8_t  memPool[3 * 1048576];

	HeapInit(memPool, 2097152);
	assert((p0 = (uint8_t*)HeapAlloc(512000)) != NULL);
	memset(p0, 0, 512000);
	assert((p1 = (uint8_t*)HeapAlloc(511000)) != NULL);
	memset(p1, 0, 511000);
	assert((p2 = (uint8_t*)HeapAlloc(26000)) != NULL);
	memset(p2, 0, 26000);
	HeapDone(&pendingBlk);
	assert(pendingBlk == 3);


	HeapInit(memPool, 2097152);
	assert((p0 = (uint8_t*)HeapAlloc(1000000)) != NULL);
	memset(p0, 0, 1000000);
	assert((p1 = (uint8_t*)HeapAlloc(250000)) != NULL);
	memset(p1, 0, 250000);
	assert((p2 = (uint8_t*)HeapAlloc(250000)) != NULL);
	memset(p2, 0, 250000);
	assert((p3 = (uint8_t*)HeapAlloc(250000)) != NULL);
	memset(p3, 0, 250000);
	assert((p4 = (uint8_t*)HeapAlloc(50000)) != NULL);
	memset(p4, 0, 50000);
	assert(HeapFree(p2));
	assert(HeapFree(p4));
	assert(HeapFree(p3));
	assert(HeapFree(p1));
	assert((p1 = (uint8_t*)HeapAlloc(500000)) != NULL);
	memset(p1, 0, 500000);
	assert(HeapFree(p0));
	assert(HeapFree(p1));
	HeapDone(&pendingBlk);
	assert(pendingBlk == 0);


	HeapInit(memPool, 2359296);
	assert((p0 = (uint8_t*)HeapAlloc(1000000)) != NULL);
	memset(p0, 0, 1000000);
	assert((p1 = (uint8_t*)HeapAlloc(500000)) != NULL);
	memset(p1, 0, 500000);
	assert((p2 = (uint8_t*)HeapAlloc(500000)) != NULL);
	memset(p2, 0, 500000);
	assert((p3 = (uint8_t*)HeapAlloc(500000)) == NULL);
	assert(HeapFree(p2));
	assert((p2 = (uint8_t*)HeapAlloc(300000)) != NULL);
	memset(p2, 0, 300000);
	assert(HeapFree(p0));
	assert(HeapFree(p1));
	HeapDone(&pendingBlk);
	assert(pendingBlk == 1);


	HeapInit(memPool, 2359296);
	assert((p0 = (uint8_t*)HeapAlloc(1000000)) != NULL);
	memset(p0, 0, 1000000);
	assert(!HeapFree(p0 + 1000));
	HeapDone(&pendingBlk);
	assert(pendingBlk == 1);

	return 0;
}
#endif /* __PROGTEST__ */