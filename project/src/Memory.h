#pragma once

#include "Utility/Exception.h"
#include <map>
#include <mutex>
#include <string>

#ifndef MEMORY_DEFAULT_LIMIT
#define MEMORY_DEFAULT_LIMIT 1073741824 // 1GB
#endif

DEFINE_EXCEPTION(OutOfMemoryException);
DEFINE_EXCEPTION(UnknownMemoryException);
DEFINE_EXCEPTION(MemoryLeakException);

class Memory
{
	struct Block
	{
		std::string TypeName;
		size_t TypeHash;
		uint64_t Size;
		const void *Data;
	};

	static uint64_t m_Limit;
	static std::map<void *, Block> m_Blocks;
	static uint64_t m_Allocated;
	static std::mutex m_Mutex;

public:
	static void Initialize(uint64_t limit = MEMORY_DEFAULT_LIMIT);
	static void Shutdown();

	static void Debug();

	static void *Allocate(const std::string &name, size_t hash, uint64_t size);
	static void Free(void *ptr);

	template<typename T>
	static T *Allocate(const std::string &name, size_t hash, uint64_t count = 1)
	{
		return static_cast<T *>(Allocate(name, hash, sizeof(T) * count));
	}

	template<typename T>
	static void Free(T *ptr)
	{
		Free(static_cast<void *>(ptr));
	}
};

#define MEM_ALLOC(type) Memory::Allocate<type>(typeid(type).name(), typeid(type).hash_code())
#define MEM_ALLOC_ARRAY(type, count) Memory::Allocate<type>(typeid(type).name(), typeid(type).hash_code(), count)
#define MEM_FREE(ptr) Memory::Free(ptr)

template<typename T, typename... TArgs>
static T *New(TArgs... args)
{
	const auto obj = MEM_ALLOC(T);
	new (obj) T(args...);
	return obj;
}

template<typename T>
static void Delete(T* obj)
{
	obj->~T();
	MEM_FREE(obj);
}