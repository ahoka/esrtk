#include <Debug.hh>
#include <Memory.hh>

#include <X86/Parameters.hh>
#include <X86/Acpi.hh>

#include <cstdio>

Rsdp *
Acpi::findRsdp(char* from, char* to)
{
   for (char* mem = from; mem < to; mem += 16)
   {
      if (mem[0] == 'R' && mem[1] == 'S' && mem[2] == 'D' &&
          mem[3] == ' ' && mem[4] == 'P' && mem[5] == 'T' &&
          mem[6] == 'R' && mem[7] == ' ')
      {  
         Rsdp* rsdp = (Rsdp* )mem;

	 if (rsdp->calculateChecksum() != 0)
	 {
            Debug::warning("Found RSDP with invalid checksum!\n");
	 }
	 else
	 {
	    if (rsdp->calculateExtendedChecksum() == 0)
	    {
               Debug::info("Found ACPI 2.0+ RSDP\n");
	    }
	    else
	    {
               Debug::info("Found ACPI 1.0 RSDP\n");
	    }

	    return rsdp;
	 }
      }
   }

   return 0;
}

void
Acpi::printAllDescriptors()
{
   uint32_t entries[256];
   char* mem = (char* )Memory::mapRegion(0x0e0000, 0x20000);

   Rsdp* rsdp = Acpi::findRsdp(mem, mem + 0x20000);
   if (rsdp == 0)
   {
      Debug::warning("ACPI RSDP not found.\n");
      return;
   }
   else
   {
      Debug::info("RSDP found at %p\n", (void*)((uintptr_t)rsdp - (uintptr_t)mem + 0x0e0000u));
   }

   printf("\n");
   rsdp->print();

   Rsdt rsdt;

   Memory::readPhysicalMemory(static_cast<void*>(&rsdt),
                              reinterpret_cast<const void*>(rsdp->rsdtAddress),
                              sizeof(rsdt));

   printf("\n");
   rsdt.print();

   if (rsdt.length == 0)
   {
      Debug::error("Error: Zero length RSDT!\n");
      return;
   }

   unsigned int rsdtSize = rsdt.length - sizeof(DescriptionHeader);
   if (rsdtSize > sizeof (entries))
   {
      Debug::error("Error: Entry size is too large to read ACPI table!\n");
      return;
   }

   Memory::readPhysicalMemory(static_cast<void*>(entries),
                              reinterpret_cast<const void*>(rsdp->rsdtAddress + sizeof (DescriptionHeader)),
                              sizeof (entries));

   Memory::unmapRegion((uintptr_t)mem, 0x20000);

   DescriptionHeader ds;
   for (unsigned int i = 0; i < rsdtSize / 4; i++)
   {
      Memory::readPhysicalMemory(reinterpret_cast<void*>(&ds),
                                 reinterpret_cast<const void*>(entries[i]),
                                 sizeof(ds));
      ds.print();
      printf("\n");
   }
}
