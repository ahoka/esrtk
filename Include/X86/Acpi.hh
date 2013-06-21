#ifndef ACPI_HH
#define ACPI_HH

#include <stdint.h>

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
} __attribute__((packed));

class Acpi
{
public:
   static Rsdt* findRsdt();
};

#endif
