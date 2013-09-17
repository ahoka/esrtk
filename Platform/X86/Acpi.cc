#include <X86/Memory.hh>
#include <X86/Parameters.hh>
#include <X86/Acpi.hh>
#include <cstdio>

Rsdp *
Acpi::findRsdp(char* from, char* to)
{
//   for (char* mem = (char*)0x0e0000; mem < (char*)0x0fffff; mem += 16)
   for (char* mem = from; mem < to; mem += 16)
   {
      if (mem[0] == 'R' && mem[1] == 'S' && mem[2] == 'D' &&
          mem[3] == ' ' && mem[4] == 'P' && mem[5] == 'T' &&
          mem[6] == 'R' && mem[7] == ' ')
      {  
         Rsdp* rsdp = (Rsdp* )mem;

	 if (rsdp->calculateChecksum() != 0)
	 {
	    printf("Found RSDP with invalid checksum!\n");
	 }
	 else
	 {
	    if (rsdp->calculateExtendedChecksum() == 0)
	    {
	       printf("Found ACPI 2.0+ RSDP\n");
	    }
	    else
	    {
	       printf("Found ACPI 1.0 RSDP\n");
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
      printf("ACPI RSDP not found.\n");
      return;
   }

   Rsdt rsdt;

   Memory::readPhysicalMemory(static_cast<void*>(&rsdt),
                              reinterpret_cast<const void*>(rsdp->rsdtAddress),
                              sizeof(rsdt));

   printf("\n");
   rsdt.printHeader();
   printf("\n");

   unsigned int rsdtSize = rsdt.length - sizeof(DescriptionHeader);
   if (rsdtSize > sizeof (entries))
   {
      printf("Error: Entry size is too large to read ACPI table!\n");
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
      ds.printHeader();
      printf("\n");
   }
}
