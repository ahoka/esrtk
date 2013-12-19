#include <Hal.hh>
#include <Thread.hh>
#include <Parameters.hh>

#include <cstdint>

/**
   uint32_t rip;
   uint32_t cs;
   uint32_t eflags;
   uint32_t useresp;
   uint32_t ss;
*/

uintptr_t
Hal::initKernelStack(uintptr_t top)
{
   uint32_t* sp = reinterpret_cast<uint32_t*>(top);

   sp[-5] = reinterpret_cast<uint32_t>(&Thread::main);
   sp[-4] = KernelCodeSegment;
   sp[-3] = 0x2; // XXX magic number
   sp[-2] = 0; // these should be discarded
   sp[-1] = 0; //

   return top - 4 * sizeof(uint32_t);
}
