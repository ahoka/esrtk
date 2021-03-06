#include <Debug.hh>
#include <Memory.hh>

#include <X86/Parameters.hh>
#include <X86/Ebda.hh>
#include <X86/Bios.hh>

#include <X86/LocalApic.hh>
#include <X86/IoApic.hh>

#include <cstring>
#include <cstdio>
#include <cassert>

#include <X86/Acpi.hh>

// Driver object
Acpi acpi;
//

int
Acpi::probe()
{
   Rsdp copy;

   if (findRsdp(&copy) != 0)
   {
      return 100;
   }

   return 0;
}

bool
Acpi::init()
{
   Rsdp* rsdp = findRsdp(&rsdpM);
   
   if (rsdp == 0)
   {
      return false;
   }

   printAllDescriptors();

   return true;
}

bool
Acpi::finalize()
{
   return true;
}

const char*
Acpi::name() const
{
   return "acpi";
}

Rsdp*
Acpi::findRsdp(Rsdp* rsdpCopy)
{
   assert(rsdpCopy != 0);

   uintptr_t ebdaAddress = Ebda::getEbda();

   Debug::verbose("EBDA: %p\n", (void*)ebdaAddress);

   char* ebda = (char* )Memory::mapRegion(ebdaAddress, ebdaAddress + 0x400);

   Rsdp* rsdp = Acpi::findRsdpSignature(ebda, ebda + 0x400u);
   if (rsdp == 0)
   {
      char* mem = (char* )Memory::mapRegion(X86_BIOS_ROM_START, X86_BIOS_ROM_SIZE);

      rsdp = Acpi::findRsdpSignature(mem, mem + X86_BIOS_ROM_SIZE);
      if (rsdp != 0)
      {
         Debug::info("RSDP found in EBDA at %p\n", (void*)((uintptr_t)rsdp - (uintptr_t)mem + 0x0e0000u));
         memcpy(rsdpCopy, rsdp, sizeof(Rsdp));
         rsdp = rsdpCopy;
      }

      Memory::unmapRegion((uintptr_t)mem, 0x20000);
   }
   else
   {
      Debug::info("RSDP found in ROM at %p\n", (void*)((uintptr_t)rsdp - (uintptr_t)ebda + ebdaAddress));
      memcpy(rsdpCopy, rsdp, sizeof(Rsdp));
      rsdp = rsdpCopy;
   }

   Memory::unmapRegion((uintptr_t)ebda, 0x400);

   return rsdp;
}

Rsdp *
Acpi::findRsdpSignature(char* from, char* to)
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

static void printMadtIntiFlags(uint16_t flags)
{
   uint8_t polarity = MpsIntiFlags::getPolarity(flags);
   uint8_t triggerMode = MpsIntiFlags::getTriggerMode(flags);

   const char* pol = "Unknown";
   if (polarity == MpsIntiFlags::ActiveHigh)
   {
      pol = "Active High";
   }
   else if (polarity == MpsIntiFlags::ActiveLow)
   {
      pol = "Active Low";
   }
   else if (polarity == MpsIntiFlags::Conforming)
   {
      pol = "Conforming";
   }

   const char* tm = "Unknown";
   if (triggerMode == MpsIntiFlags::EdgeTriggered)
   {
      tm = "Edge-Triggered";
   }
   else if (triggerMode == MpsIntiFlags::LevelTriggered)
   {
      tm = "Level-Triggered";
   }
   else if (triggerMode == MpsIntiFlags::Conforming)
   {
      tm = "Conforming";
   }

   Debug::verbose("Trigger mode: %s, %s\n", pol, tm);
}

static void
readMadt(Madt* madt, size_t size)
{
   Debug::verbose("LAPIC Address: %p\n", (void*)madt->lapicAddress);
   Debug::verbose("Flags: 0x%x\n", madt->flags);

   new LocalApic(madt->lapicAddress);

   static_assert(sizeof(Madt) == 44, "MADT struct size is incorrent");
   auto offset = sizeof(Madt);
   while (offset < size)
   {
      auto controller = (MadtInterruptController*)((uintptr_t)madt + offset);

      if (controller->type == MadtInterruptController::Lapic)
      {
         auto lapic = (MadtLocalApic*)controller;

         Debug::verbose("LAPIC: Length: %zu, Processor ID: %u, Apic ID: %u, Flags: 0x%x\n",
                (size_t)lapic->length, lapic->processorId,
                lapic->apicId, lapic->flags);

         assert(lapic->length == 8);
      }
      else if (controller->type == MadtInterruptController::IoApic)
      {
         auto ioapic = (MadtIoApic*)controller;
         assert(ioapic->length == 12);

         Debug::verbose("I/O APIC: Length: %zu, I/O Apic Id: %u, I/O Apic Address: %p, GSI Base: %u\n",
                (size_t)ioapic->length,
                ioapic->ioApicId,
                (void*)ioapic->ioApicAddress,
                ioapic->gsiBase);

         new IoApic(ioapic->ioApicAddress, ioapic->ioApicId);
      }
      else if (controller->type == MadtInterruptController::Override)
      {
         auto override = (MadtInputSourceOverride*)controller;

         Debug::verbose("Override: Length: %zu, Bus: %hhu, Source: %hhu, GSI: %u, Flags: 0x%x\n",
                (size_t)override->length, override->bus, override->source,
                override->gsi, override->flags);

         printMadtIntiFlags(override->flags);

         assert(override->length == 10);
      }
      else if (controller->type == MadtInterruptController::NmiSource)
      {
         auto nmiSource = (MadtNmiSource*)controller;

         Debug::verbose("NMI Source: Length: %zu, Flags: 0x%hx, GSI: %u\n",
                (size_t)nmiSource->length, nmiSource->flags,
		nmiSource->gsi);

         printMadtIntiFlags(nmiSource->flags);
      }
      else if (controller->type == MadtInterruptController::LapicNmi)
      {
         auto lapicNmi = (MadtLapicNmi*)controller;

         Debug::verbose("LAPIC NMI: Length: %zu, ACPI Processor ID: %hhu, "
                        "Flags: 0x%hx, LAPIC LINT: %hhu\n", (size_t)lapicNmi->length,
                        lapicNmi->processorId, lapicNmi->flags, lapicNmi->lapicLint);

         printMadtIntiFlags(lapicNmi->flags);
      }
      else 
      {
         Debug::verbose("Type: 0x%x, Length: %zu\n", controller->type, (size_t)controller->length);
      }

      offset += controller->length;
   }

   Debug::verbose("\n");
}

void
Acpi::printAllDescriptors()
{
   uint32_t entries[256];

//   assert(rsdpM != 0);

   rsdpM.print();

   Rsdt rsdt;

   Memory::readPhysicalMemory(static_cast<void*>(&rsdt),
                              reinterpret_cast<const void*>(rsdpM.rsdtAddress),
                              sizeof(rsdt));

   Debug::verbose("\n");
   rsdt.print();
   Debug::verbose("\n");

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
                              reinterpret_cast<const void*>(rsdpM.rsdtAddress + sizeof (DescriptionHeader)),
                              sizeof (entries));

   DescriptionHeader ds;
   for (unsigned int i = 0; i < rsdtSize / 4; i++)
   {
      Memory::readPhysicalMemory(reinterpret_cast<void*>(&ds),
                                 reinterpret_cast<const void*>(entries[i]),
                                 sizeof(ds));
      ds.print();
      Debug::verbose("\n");

      if (ds.signature[0] == 'A' && ds.signature[1] == 'P' &&
          ds.signature[2] == 'I' && ds.signature[3] == 'C')
      {
         Madt* madt = (Madt*)Memory::mapRegion(entries[i], ds.length + sizeof(Madt));

         printf("mapped to %p\n", (void*)madt);
         readMadt(madt, ds.length);

         Memory::unmapRegion((uintptr_t)madt, ds.length + sizeof(Madt));
      }
   }
}
