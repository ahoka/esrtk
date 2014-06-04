#include <Memory.hh>
#include <X86/Bios.hh>
#include <X86/Ebda.hh>

uintptr_t
Ebda::getEbda()
{
   static uintptr_t ebda;

   if (ebda != 0)
   {
      return ebda;
   }

   uint16_t ebdaPointer;
   Memory::readPhysicalMemory(&ebdaPointer, (void*)X86_BIOS_EBDA_ADDRESS, sizeof(ebdaPointer));

   ebda = uintptr_t(ebdaPointer) << 4u;

   return ebda;
}
