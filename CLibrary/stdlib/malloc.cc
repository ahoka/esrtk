#include <MemoryManager.hh>

#include <cstddef>

extern "C" void *
malloc(size_t size)
{
   return MemoryManager::get().allocate(size);
}

extern "C" void
free(void* data)
{
   MemoryManager::get().deallocate(data);
}

extern "C" void *
realloc(void* data, size_t size)
{
   return MemoryManager::get().reallocate(data, size);
}
