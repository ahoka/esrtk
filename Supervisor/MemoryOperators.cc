#include <MemoryManager.hh>

#include <cstddef>
#include <new>

void *operator new(std::size_t size)
{
   return MemoryManager::get().allocate(size);
}
 
void *operator new[](std::size_t size)
{
   return MemoryManager::get().allocate(size);
}
 
void operator delete(void* p)
{
   MemoryManager::get().deallocate(p);
}
 
void operator delete[](void* p)
{
   MemoryManager::get().deallocate(p);
}

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
