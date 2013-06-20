#ifndef APIC_HH
#define APIC_HH

#include <stdint.h>

class Apic
{
public:
   void init();
   void printInformation();
   int probe();

   void read32(uint32_t offset, uint32_t *a);
   void read64(uint32_t offset, uint32_t *a, uint32_t *b);
   void write32(uint32_t offset, uint32_t a);
   void write64(uint32_t offset, uint32_t a, uint32_t b);

private:
   uint32_t apicAddress;
   bool isBsp;
   bool isEnabled;
};

extern Apic apic;

#endif
