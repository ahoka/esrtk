#include <X86/Parameters.hh>
#include <X86/Memory.hh>

uint32_t Memory::heapEnd = HeapStart;
uint32_t Memory::stackEnd = StackStart;

void
Memory::init()
{
   //...
}

bool
Memory::handlePageFault(uint32_t address)
{
   (void )address;

   return false;
}
