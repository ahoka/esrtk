#include "MemoryManager.hh"
#include <Memory.hh>
#include <Debug.hh>

void*
MemoryManager::allocateBackend(std::size_t size)
{
   std::size_t rsize = roundTo<uintptr_t>(size, PageSize);
   void* data = reinterpret_cast<void*>(Memory::sbrk(rsize));

   KASSERT(data != 0);

   return data;
}

void*
MemoryManager::allocate(std::size_t size)
{
   std::size_t rsize = roundTo<uintptr_t>(size, PageSize);

   return allocateBackend(rsize);
}

void
MemoryManager::deallocate(void */*data*/)
{
   return;
}
