#include <MemoryManager.hh>

#include <Memory.hh>
#include <Debug.hh>

#include <cstdio>

MemoryManager* MemoryManager::self = 0;

MemoryManager::MemoryManager()
{
   // empty!
}

void*
MemoryManager::operator new(std::size_t /*size*/, void *placement)
{
   return placement;
}

void
MemoryManager::init()
{
   printf("Intializing Memory Manager\n");
   void* object = allocateBackend(sizeof(MemoryManager));

   MemoryManager::self = new (object) MemoryManager();

   printf("Memory Manager at %p\n", (void* )MemoryManager::self);
}

MemoryManager&
MemoryManager::get()
{
   return *self;
}

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
