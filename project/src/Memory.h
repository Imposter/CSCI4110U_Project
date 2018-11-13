#pragma once

#include "Utility/Exception.h"
#include <cstdint>
#include <string>

#ifndef MEMORY_DEFAULT_LIMIT
#define MEMORY_DEFAULT_LIMIT (1024 * 1024 * 1024) // 1GB
#endif

DEFINE_EXCEPTION(OutOfMemoryException);
DEFINE_EXCEPTION(UnknownMemoryException);
DEFINE_EXCEPTION(MemoryLeakException);

void MemoryInitialize(uint64_t limit = MEMORY_DEFAULT_LIMIT);
void MemoryShutdown();

void MemoryDebug();

void *MemoryAllocate(const std::string &name, size_t hash, uint64_t size);
void MemoryFree(void *ptr);

template<typename T>
static T *MemoryAllocate(const std::string &name, size_t hash, uint64_t count = 1)
{
	return static_cast<T *>(MemoryAllocate(name, hash, sizeof(T) * count));
}

template<typename T>
static void MemoryFree(T *ptr)
{
	MemoryFree(static_cast<void *>(ptr));
}

#define MEM_ALLOC(type) MemoryAllocate<type>(typeid(type).name(), typeid(type).hash_code())
#define MEM_ALLOC_ARRAY(type, count) MemoryAllocate<type>(typeid(type).name(), typeid(type).hash_code(), count)
#define MEM_DELETE(ptr) MemoryFree(ptr)

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
	MEM_DELETE(obj);
}