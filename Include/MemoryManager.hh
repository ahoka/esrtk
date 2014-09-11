#ifndef MEMORYMANAGER_HH
#define	MEMORYMANAGER_HH

#include <Templates.hh>
#include <Parameters.hh>

#include <cstdint>
#include <cstddef>

#include <Debug.hh>

#include <Segment.hh>
#include <SegmentList.hh>

class MemoryManager
{
public:
   MemoryManager();
   ~MemoryManager();

   static void init();

   void* allocate(std::size_t size);
   void deallocate(void *data);
   void* reallocate(void* data, std::size_t size);

   void printStatistics();

   static MemoryManager& get();

private:
   MemoryManager(const MemoryManager& orig);

   void* operator new(std::size_t, void *);

   static void* allocateBackend(std::size_t size);

   static Segment* headerOf(void* segment);

   SegmentList freeList;

   static MemoryManager* self;
};

#endif	/* MEMORYMANAGER_H */

