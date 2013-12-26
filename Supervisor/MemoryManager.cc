#include <MemoryManager.hh>

#include <Memory.hh>
#include <Debug.hh>

#include <cstdio>

#define DEBUG

MemoryManager* MemoryManager::self = 0;

MemoryManager::MemoryManager()
   : freeList()
{
   // empty!
}

void*
MemoryManager::operator new(std::size_t /*size*/, void *placement)
{
   return placement;
}

MemoryManager::Segment*
MemoryManager::headerOf(void* data)
{
   return reinterpret_cast<Segment*>(uintptr_t(data) - sizeof(Segment));
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
   std::size_t rsize = roundTo(size, PageSize);

   void* data = reinterpret_cast<void*>(Memory::mapAnonymousRegion(rsize));

   KASSERT(data != 0);

   return data;
}

void*
MemoryManager::allocate(std::size_t size)
{
   for (auto& s : freeList)
   {
      if (s.getSize() >= size)
      {
	 printf("Found item with size: %zu\n", s.getSize());
	 freeList.remove(&s);
	 s.updateChecksum();
	 s.markAllocated();
#ifdef DEBUG
	 printf("Debug: allocating from freelist\n");
#endif
	 return reinterpret_cast<void*>(s.getAddress());
      }
   }

#ifdef DEBUG
   printf("Debug: allocating from heap\n");
#endif

   std::size_t rsize = roundTo(size + sizeof(Segment), PageSize);
//   Segment* segment = reinterpret_cast<Segment*>(allocateBackend(rsize));
   Segment* segment = new (reinterpret_cast<void*>(allocateBackend(rsize))) Segment;

   segment->setSize(rsize - sizeof(Segment));
   segment->setAddress(reinterpret_cast<uintptr_t>(segment + 1));
   segment->markAllocated();

   return reinterpret_cast<void*>(segment->getAddress());
}

void
MemoryManager::deallocate(void *data)
{
   Segment* segment = headerOf(data);

   KASSERT(segment->isAllocated());
   segment->markAllocated();

#ifdef DEBUG
   printf("Putting to freelist\n");
#endif

   freeList.insertLast(segment);
   segment->updateChecksum();
}

void
MemoryManager::printStatistics()
{
   printf("Allocator statistics:\n");
   printf("Free segments: %zu\n", freeList.getCount());

   for (auto& s : freeList)
   {
      printf("Segment %p: size: %zu, address: %p\n", &s, s.getSize(), (void* )s.getAddress());
   }
}

