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
   const std::size_t initSize = (5 * 4) + (12 * 4);

   uint32_t* sp = reinterpret_cast<uint32_t*>(top - initSize);

   sp[12] = reinterpret_cast<uint32_t>(&Thread::main);
   sp[13] = KernelCodeSegment;
   sp[14] = 0x2; // XXX magic number
   sp[15] = 0; // these should be discarded
   sp[16] = 0; //

   sp[11] = 0x0; //
   sp[10] = 0x0; //
   sp[9] = 0x0; //
   sp[8] = 0x0; //
   sp[7] = 0x0; //
   sp[6] = 0x0; //
   sp[5] = 0x0; //
   sp[4] = 0x0; //
   sp[3] = KernelDataSegment; //
   sp[2] = KernelDataSegment; //
   sp[1] = KernelDataSegment; //
   sp[0] = KernelDataSegment; //

   // subtract the above and the registers
   return top - initSize;
}
