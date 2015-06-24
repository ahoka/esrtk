#include <Kernel/Heap.hh>

#include <Memory.hh>
#include <Debug.hh>
#include <spinlock.h>

#include <cstdio>
#include <cstring>

using namespace Kernel;

static spinlock_softirq_t heapLock = SPINLOCK_SOFTIRQ_STATIC_INITIALIZER;

#define DEBUG

Heap::Heap()
   : freeList()
{
   // empty!
}

Heap::~Heap()
{
   KASSERT(0);
}

void*
Heap::operator new(std::size_t /*size*/, void *placement)
{
   return placement;
}

Segment*
Heap::headerOf(void* data)
{
   return reinterpret_cast<Segment*>(uintptr_t(data) - sizeof(Segment));
}

void
Heap::init()
{
   printf("Intializing Memory Manager\n");
}

Heap&
Heap::get()
{
   static Heap heap;
   return heap;
}

void*
Heap::allocateBackend(std::size_t size)
{
   std::size_t rsize = roundTo(size, PageSize);

   void* data;
   data = reinterpret_cast<void*>(Memory::mapAnonymousRegion(rsize));

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
Heap::allocate(std::size_t size)
{
   spinlock_softirq_enter(&heapLock);
#ifdef DEBUG
   printf("Allocating %zu, FreeList items: %lu\n", size, freeList.count());
#endif

   for (auto s = freeList.begin(); s != freeList.end(); ++s)
   {
      s->dump();

      if (s->getSize() >= size)
      {
	 printf("Found item with size: %zu\n", s->getSize());
	 freeList.remove(s);
         spinlock_softirq_exit(&heapLock);

	 s->updateChecksum();
	 s->markAllocated();
#ifdef DEBUG
	 printf("Debug: allocating from freelist: %p\n", (void*)s->getAddress());
#endif
         std::memset((void*)s->getAddress(), 0, s->getSize());
	 return reinterpret_cast<void*>(s->getAddress());
      }
   }

   spinlock_softirq_exit(&heapLock);

#ifdef DEBUG
   printf("Debug: allocating from heap\n");
#endif

   std::size_t rsize = roundTo(size + sizeof(Segment), PageSize);
//   Segment* segment = reinterpret_cast<Segment*>(allocateBackend(rsize));
   Segment* segment = new (allocateBackend(rsize)) Segment;

   segment->setSize(rsize - sizeof(Segment));
   segment->markAllocated();
   segment->updateChecksum();

#ifdef DEBUG
   printf("Debug: allocated %p (%p)\n", (void*)segment, reinterpret_cast<void*>(segment->getAddress()));
#endif

   return reinterpret_cast<void*>(segment->getAddress());
}

void
Heap::deallocate(void *data)
{
   Segment* segment = headerOf(data);

#ifdef DEBUG
   printf("Debug: deallocating: %p (%p)\n", (void*)segment, data);
#endif

   KASSERT(segment->isAllocated());
   segment->markUnallocated();

#ifdef DEBUG
   printf("Putting to freelist\n");
#endif

   spinlock_softirq_enter(&heapLock);

   freeList.add(segment);
   segment->updateChecksum();

   spinlock_softirq_exit(&heapLock);

#ifdef DEBUG
   printf("Debug: deallocated\n");
#endif
}

void*
Heap::reallocate(void* data, std::size_t size)
{
   Segment* segment = headerOf(data);
   auto oldSize = segment->getSize();

   if (size == 0)
   {
      deallocate(data);

      return 0;
   }
   else
   {
      void* newData = allocate(size);

      if (data != 0)
      {
         memcpy(newData, data, oldSize);
         deallocate(data);
      }

      return newData;
   }
}

void
Heap::printStatistics()
{
   printf("Allocator statistics:\n");

   auto count = freeList.count();
   printf("Free segments: %lu\n", count);
}
