#ifndef ACPI_HH
#define ACPI_HH

#include <stdint.h>
#include <stdio.h>

struct Rsdt
{
   uint8_t signature[8];
   uint8_t checksum;
   uint8_t oemId[6];
   uint8_t revision;
   uint32_t rsdtAddress;
   uint32_t length;
   uint64_t xsdtAddress;
   uint8_t extendedChecksum;
   uint8_t _reserved[3];

   uint8_t calculateChecksum()
   {
      uint8_t sum = 0;

      unsigned int size = (int )((uint8_t *)&xsdtAddress - (uint8_t *)this);

      uint8_t* rsdt = (uint8_t *)this;
      for (unsigned int i = 0; i < size; i++)
      {
	 sum += rsdt[i];
      }

      return sum;
   }

   uint8_t calculateExtendedChecksum()
   {
      uint8_t sum = 0;

      uint8_t* rsdt = (uint8_t *)this;
      for (unsigned int i = 0; i < sizeof(this); i++)
      {
	 sum += rsdt[i];
      }

      printf("sz: %u, cksum %hhu\n", sizeof(this), sum);
      return sum;
   }
} __attribute__((packed));

class Acpi
{
public:
   static Rsdt* findRsdt();
};

#endif
