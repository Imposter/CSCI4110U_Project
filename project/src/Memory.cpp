#include "Memory.h"
#include "Log.h"

uint64_t Memory::m_Limit;
std::map<void *, Memory::Block> Memory::m_Blocks;
uint64_t Memory::m_Allocated;
std::mutex Memory::m_Mutex;

void Memory::Initialize(uint64_t limit)
{
	m_Limit = limit;
}

void Memory::Shutdown()
{
	// If there's still memory allocated there's an issue
	if (!m_Blocks.empty())
		THROW_EXCEPTION(MemoryLeakException, "Memory leak detected (%u blocks, %llu/%llu)", m_Blocks.size(), m_Allocated, m_Limit);
}

void Memory::Debug()
{
	LOG_TRACE("Memory", "Dumping allocated memory (%d blocks, %llu/%llu)", m_Blocks.size(), m_Allocated, m_Limit);
	m_Mutex.lock();
	for (auto &pair : m_Blocks)
	{
		const auto &region = pair.second;
		LOG_TRACE("Memory", "- Block 0x%X of size %llu and type \"%s\" (%u)", pair.first, region.Size, region.TypeName.c_str(), region.TypeHash);
	}
	m_Mutex.unlock();
}

void *Memory::Allocate(const std::string &name, size_t hash, uint64_t size)
{
	m_Mutex.lock();

	// Check if we would be out of memory due to this allocation
	if (m_Limit != 0 && m_Allocated + size > m_Limit)
	{
		m_Mutex.unlock();
		THROW_EXCEPTION(OutOfMemoryException, "Out of memory (%llu/%llu)", m_Allocated, m_Limit);
	}

	// Allocate memory and store region
	const auto ptr = malloc(size);
	memset(ptr, 0, size);

	m_Allocated += size;
	m_Blocks.insert({ ptr, { name, hash, size, ptr } });
	m_Mutex.unlock();

	return ptr;
}

void Memory::Free(void *ptr)
{
	m_Mutex.lock();

	// Find region
	std::map<void *, Block>::iterator it;
	if ((it = m_Blocks.find(ptr)) == m_Blocks.end())
	{
		m_Mutex.unlock();
		THROW_EXCEPTION(UnknownMemoryException, "Unknown memory block 0x%X", ptr);
	}

	// Free memory and region region
	free(ptr);

	const auto &region = it->second;
	m_Allocated -= region.Size;
	m_Blocks.erase(it);

	m_Mutex.unlock();
}
