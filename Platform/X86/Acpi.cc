#include <X86/Acpi.hh>

#include <Debug.hh>
#include <Memory.hh>

#include <X86/Parameters.hh>
#include <X86/Ebda.hh>
#include <X86/Bios.hh>

#include <cstring>
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

static void readMadt(uintptr_t madtAddress, size_t size)
{
   auto start = Memory::mapRegion(madtAddress, madtAddress + size);

   Madt* madt = (Madt* )start;

   printf("LAPIC Address: %p\n", (void*)madt->lapicAddress);
   printf("Flags: 0x%x\n", madt->flags);

   for (auto remaining = size - sizeof(Madt); remaining > 0;)
   {
      auto controller = (MadtInterruptController*)(start + remaining);
      printf("Type: 0x%x, Length: %zu\n", controller->type, (size_t)controller->length);
      printf("Remaining: %zu\n", remaining);

      if (controller->type == MadtInterruptController::LAPIC)
      {
         auto lapic = (MadtLocalApic*)controller;

         printf("LAPIC: Processor ID: %u, Apic ID: %u, Flags: 0x%x\n", 
                lapic->processorId, lapic->apicId, lapic->flags);
      }

      if (remaining > controller->length)
      {
         remaining =- controller->length;
      }
      {
         printf("MADT size mismatch?\n");
         break;
      }
   }

   printf("\n");

   Memory::unmapRegion((uintptr_t)madt, (uintptr_t)madt + size);
}

void
Acpi::printAllDescriptors()
{
   uint32_t entries[256];
   uintptr_t ebdaAddress;
   Rsdp rsdpCopy;

   ebdaAddress = Ebda::getEbda();

   printf("EBDA: %p\n", (void*)ebdaAddress);

   char* ebda = (char* )Memory::mapRegion(ebdaAddress, ebdaAddress + 0x400);

   Rsdp* rsdp = Acpi::findRsdp(ebda, ebda + 0x400u);
   if (rsdp == 0)
   {
      char* mem = (char* )Memory::mapRegion(X86_BIOS_ROM_START, X86_BIOS_ROM_SIZE);

      rsdp = Acpi::findRsdp(mem, mem + X86_BIOS_ROM_SIZE);
      if (rsdp != 0)
      {
         Debug::info("RSDP found in EBDA at %p\n", (void*)((uintptr_t)rsdp - (uintptr_t)mem + 0x0e0000u));
         memcpy(&rsdpCopy, rsdp, sizeof(Rsdp));
         rsdp = &rsdpCopy;
      }

      Memory::unmapRegion((uintptr_t)mem, 0x20000);
   }
   else
   {
      Debug::info("RSDP found in ROM at %p\n", (void*)((uintptr_t)rsdp - (uintptr_t)ebda + ebdaAddress));
      memcpy(&rsdpCopy, rsdp, sizeof(Rsdp));
      rsdp = &rsdpCopy;
   }

   Memory::unmapRegion((uintptr_t)ebda, 0x400);

   if (rsdp == 0)
   {
      Debug::warning("ACPI RSDP not found.\n");
      return;
   }
   else
   {

   }

   printf("\n");
   rsdp->print();

   Rsdt rsdt;

   Memory::readPhysicalMemory(static_cast<void*>(&rsdt),
                              reinterpret_cast<const void*>(rsdp->rsdtAddress),
                              sizeof(rsdt));

   printf("\n");
   rsdt.print();
   printf("\n");

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

   DescriptionHeader ds;
   for (unsigned int i = 0; i < rsdtSize / 4; i++)
   {
      Memory::readPhysicalMemory(reinterpret_cast<void*>(&ds),
                                 reinterpret_cast<const void*>(entries[i]),
                                 sizeof(ds));
      ds.print();
      printf("\n");

      if (ds.signature[0] == 'A' && ds.signature[1] == 'P' &&
          ds.signature[2] == 'I' && ds.signature[3] == 'C')
      {
         readMadt(entries[i], ds.length);
      }
   }
}
