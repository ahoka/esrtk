#ifndef HEAP_HH
#define	HEAP_HH

#include <Templates.hh>
#include <Parameters.hh>

#include <cstdint>
#include <cstddef>

#include <Debug.hh>

#include <Segment.hh>
#include <SegmentList.hh>

namespace Kernel
{

class Heap
{
public:
   Heap();
   ~Heap();

   static void init();

   void* allocate(std::size_t size);
   void deallocate(void *data);
   void* reallocate(void* data, std::size_t size);

   void printStatistics();

   static Heap& get();

private:
   Heap(const Heap& orig);

   void* operator new(std::size_t, void *);

   static void* allocateBackend(std::size_t size);

   static Segment* headerOf(void* segment);

   SegmentList freeList;

   static Heap* self;
};

}

#endif	/* MEMORYMANAGER_H */

