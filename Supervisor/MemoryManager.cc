#include <MemoryManager.hh>

#include <Memory.hh>
#include <Debug.hh>

#include <cstdio>
#include <cstring>

//#define DEBUG

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

Segment*
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

   void* data;
   if (rsize == PageSize)
   {
      uintptr_t page = Memory::getPage();
      KASSERT(page != 0);

      data = reinterpret_cast<void*>(Memory::mapPage(page));
   }
   else
   {
      data = reinterpret_cast<void*>(Memory::mapAnonymousRegion(rsize));
   }

#ifdef DEBUG
   printf("Allocating from backend: %p\n", data);
#endif

   KASSERT(data != 0);

   // zero memory to avoid leaking anything
   //
   std::memset(data, 0, rsize);

   return data;
}

void*
MemoryManager::allocate(std::size_t size)
{
//   for (auto& s : freeList)
//   for (Segment* s = freeList.nextSegment; s.getSize() != 0; s = s.nextSegment)
   auto iterator = freeList.getIterator();
   while (iterator.hasNext())
   {
      auto s = iterator.getNext();

      if (s.getSize() >= size)
      {
	 printf("Found item with size: %zu\n", s.getSize());
//	 freeList.remove(&s);
	 iterator.remove();
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
   Segment* segment = new (allocateBackend(rsize)) Segment;

   segment->setSize(rsize - sizeof(Segment));
   segment->markAllocated();

#ifdef DEBUG
   printf("Debug: allocated %p (%p)\n", (void*)segment, reinterpret_cast<void*>(segment->getAddress()));
#endif

   return reinterpret_cast<void*>(segment->getAddress());
}

void
MemoryManager::deallocate(void *data)
{
   Segment* segment = headerOf(data);

#ifdef DEBUG
   printf("Debug: deallocating: %p (%p)\n", data, (void*)segment);
#endif

   KASSERT(segment->isAllocated());
   segment->markAllocated();

#ifdef DEBUG
   printf("Putting to freelist\n");
#endif

   freeList.add(segment);

   segment->updateChecksum();
}

void
MemoryManager::printStatistics()
{
   printf("Allocator statistics:\n");

//   for (auto s = freeList.getIterator(); s.hasNext; s = s.nextSegment)
//   for (auto& s : freeList)
   int count = 0;
   auto iterator = freeList.getIterator();
   while (iterator.hasNext())
   {
      auto s = iterator.getNext();
      count++;
      printf("Segment %p: size: %zu, address: %p\n", &s, s.getSize(), (void* )s.getAddress());
   }

   printf("Free segments: %d\n", count);
}

