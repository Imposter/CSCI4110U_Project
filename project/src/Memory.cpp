#include "Memory.h"
#include "Log.h"
#include <map>
#include <mutex>

struct Block
{
	std::string TypeName;
	size_t TypeHash;
	uint64_t Size;
	const void *Data;
};

uint64_t g_MemoryLimit;
std::map<void *, Block> g_MemoryBlocks;
uint64_t g_MemoryAllocated;
std::mutex g_MemoryMutex;

void MemoryInitialize(uint64_t limit)
{
	g_MemoryLimit = limit;
}

void MemoryShutdown()
{
	// If there's still memory allocated there's an issue
	if (!g_MemoryBlocks.empty())
		THROW_EXCEPTION(MemoryLeakException, "Memory leak detected (%u blocks, %llu/%llu)", g_MemoryBlocks.size(), g_MemoryAllocated, g_MemoryLimit);
}

void MemoryDebug()
{
	LOG_TRACE("Memory", "Dumping allocated memory (%d blocks, %llu/%llu)", g_MemoryBlocks.size(), g_MemoryAllocated, g_MemoryLimit);
	g_MemoryMutex.lock();
	for (auto &pair : g_MemoryBlocks)
	{
		const auto &region = pair.second;
		LOG_TRACE("Memory", "- Block 0x%X of size %llu and type \"%s\" (%u)", pair.first, region.Size, region.TypeName.c_str(), region.TypeHash);
	}
	g_MemoryMutex.unlock();
}

void *MemoryAllocate(const std::string &name, size_t hash, uint64_t size)
{
	g_MemoryMutex.lock();

	// Check if we would be out of memory due to this allocation
	if (g_MemoryLimit != 0 && g_MemoryAllocated + size > g_MemoryLimit)
	{
		g_MemoryMutex.unlock();
		THROW_EXCEPTION(OutOfMemoryException, "Out of memory (%llu/%llu)", g_MemoryAllocated, g_MemoryLimit);
	}

	// Allocate memory and store region
	const auto ptr = malloc(size);
	memset(ptr, 0, size);

	g_MemoryAllocated += size;
	g_MemoryBlocks.insert({ ptr, { name, hash, size, ptr } });
	g_MemoryMutex.unlock();

	return ptr;
}

void MemoryFree(void *ptr)
{
	g_MemoryMutex.lock();

	// Find region
	std::map<void *, Block>::iterator it;
	if ((it = g_MemoryBlocks.find(ptr)) == g_MemoryBlocks.end())
	{
		g_MemoryMutex.unlock();
		THROW_EXCEPTION(UnknownMemoryException, "Unknown memory block 0x%X", ptr);
	}

	// Free memory and region region
	free(ptr);

	const auto &region = it->second;
	g_MemoryAllocated -= region.Size;
	g_MemoryBlocks.erase(it);

	g_MemoryMutex.unlock();
}