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

char bitmap[4096];
Entry entries[4096];

Entry*
getFreePageEntry()
{
   for (unsigned int i = 0; i < sizeof bitmap; i++)
   {
      if (bitmap[i] == 0)
      {
	 bitmap[i] = 1;
	 return &entries[i];
      }
   }

   return 0;
}

int
initPageDirectory()
{
   for (unsigned int i = 0; i < sizeof bitmap; i++)
   {
      bitmap[i] = 0;
   }

   return 0;
}

#endif
