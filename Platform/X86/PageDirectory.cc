#include <X86/CpuRegisters.hh>

#ifdef PAE
struct Entry
{
   unsigned int low;
   unsigned int high;
};
#else
typedef unsigned long Entry;
#endif
