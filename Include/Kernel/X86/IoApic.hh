#ifndef IOAPIC_HH
#define IOAPIC_HH

#include <cstdint>

class IoApic
{
public:
   IoApic(uintptr_t address, uint32_t id);

private:
   void write(uint32_t reg, uint32_t value);
   uint32_t read(uint32_t reg);

   uint32_t idM;
   uintptr_t physicalAddressM;
   uintptr_t addressM;
};

#endif
