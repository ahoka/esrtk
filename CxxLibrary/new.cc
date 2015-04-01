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

void operator delete(void* p, std::size_t)
{
   MemoryManager::get().deallocate(p);
}
 
void operator delete[](void* p, std::size_t)
{
   MemoryManager::get().deallocate(p);
}
