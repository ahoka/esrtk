#include <Memory.hh>

#include <X86/IoApic.hh>

namespace
{
   enum
   {
      IOREGSEL = 0x00,
      IOREGWIN = 0x10
   };

   enum
   {
      IOAPICID = 0x00,
      IOAPICVER = 0x01,
      IOAPICARB = 0x02,
      IOREDTBL = 0x10
   };
};

IoApic::IoApic(uintptr_t address, uint32_t id)
   : idM(id),
     physicalAddressM(address)
{
   addressM = Memory::mapRegion(physicalAddressM, 4096u,
                                Memory::MapUncacheable);

   printf("IOAPICID: 0x%x\n", read(IOAPICID));
   printf("IOAPICVER: 0x%x\n", read(IOAPICVER));

   assert(addressM != 0);
}

void
IoApic::write(uint32_t reg, uint32_t value)
{
   *(volatile uint32_t* )(addressM + IOREGSEL) = reg;
   *(volatile uint32_t* )(addressM + IOREGWIN) = value;
}

uint32_t
IoApic::read(uint32_t reg)
{
   *(volatile uint32_t* )(addressM + IOREGSEL) = reg;
   return *(volatile uint32_t* )(addressM + IOREGWIN);
}
