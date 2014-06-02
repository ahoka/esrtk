#ifndef ACPI_HH
#define ACPI_HH

#include <cstdint>
#include <cstdio>
#include <cstddef>

struct DescriptionHeader
{
   uint8_t signature[4];
   uint32_t length;
   uint8_t revision;
   uint8_t checksum;
   uint8_t oemId[6];
   uint8_t oemTableId[8];
   uint32_t oemRevision;
   uint8_t creatorId[4];
   uint32_t creatorRevision;

   void print()
   {
      printf("Signature:\t\t%c%c%c%c\n", signature[0],
	     signature[1], signature[2], signature[3]);
      printf("Length:\t\t\t%u\n", length);
      printf("Revision:\t\t%u\n", revision);
      printf("OEM ID:\t\t\t%c%c%c%c%c%c\n",
	     oemId[0], oemId[1], oemId[2],
	     oemId[3], oemId[4], oemId[5]);
      printf("OEM Table ID:\t\t%c%c%c%c%c%c%c%c\n",
	     oemTableId[0], oemTableId[1], oemTableId[2], oemTableId[3], 
	     oemTableId[4], oemTableId[5], oemTableId[6], oemTableId[7]);
      printf("OEM Revision:\t\t0x%x\n", oemRevision);
      printf("Creator ID:\t\t%c%c%c%c\n", creatorId[0], creatorId[1],
	     creatorId[2], creatorId[3]);
      printf("Creator Revision:\t0x%x\n",
	     creatorRevision);
   }

   uint8_t calculateChecksum()
   {
      uint8_t sum = 0;

      uint8_t* table = (uint8_t *)this;
      for (unsigned int i = 0; i < length; i++)
      {
	 sum += table[i];
      }

      return sum;
   }
} __attribute__((packed));

struct Rsdt : DescriptionHeader
{
   uint32_t entry[1];
};// __attribute__((packed));

struct Xsdt : DescriptionHeader
{
   uint64_t entry[1];
};// __attribute__((packed));

struct Madt : DescriptionHeader
{
   uint32_t lapicAddress;
   uint32_t flags;
   //
   enum
   {
      PCAT_COMPAT = 0x1
   };
};

struct MadtInterruptController
{
   uint8_t type;
   uint8_t length;
   
   enum Type
   {
      LAPIC = 0x0,
      IOAPIC = 0x1,
      OVERRIDE = 0x2
   };
} __attribute__((packed));

struct MpsIntiFlags
{
   enum Polarity
   {
      Conforming = 0x0,
      ActiveHigh = 0x1,
      ActiveLow = 0x3
   };

   enum TriggerMode
   {
//      Conforming = 0x0,
      EdgeTriggered = 0x1,
      LevelTriggered = 0x3
   };

   static uint8_t getPolarity(uint16_t flags)
   {
      return flags & 0x3;
   }

   static uint8_t getTriggerMode(uint16_t flags)
   {
      return (flags >> 2) & 0x3;
   }

} __attribute__((packed));

struct MadtLocalApic : MadtInterruptController
{
   uint8_t processorId;
   uint8_t apicId;
   uint32_t flags;
};

struct MadtIoApic : MadtInterruptController
{
   uint8_t ioApicId;
   uint8_t reserved_;
   uint32_t ioApicAddress;
   uint32_t gsiBase;
};

struct MadtInputSourceOverride : MadtInterruptController
{
   uint8_t bus;
   uint8_t source;
   uint32_t gsi;
   uint16_t flags;
};

struct Rsdp
{
   uint8_t signature[8];
   uint8_t checksum;
   uint8_t oemId[6];
   uint8_t revision;
   uint32_t rsdtAddress;
   // ACPI 2.0+ from here
   uint32_t length;
   uint64_t xsdtAddress;
   uint8_t extendedChecksum;
   uint8_t _reserved[3];

   void print()
   {
      printf("Signature:\t\t%c%c%c%c%c%c%c%c\n", signature[0],
	     signature[1], signature[2], signature[3],
             signature[4], signature[5], signature[6], signature[7]);
      printf("Checksum:\t\t0x%x\n", checksum);
      printf("OEM ID:\t\t\t%c%c%c%c%c%c\n",
	     oemId[0], oemId[1], oemId[2],
	     oemId[3], oemId[4], oemId[5]);
      printf("Revision:\t\t%u\n", revision);
      printf("RSDT Address:\t\t0x%x\n", rsdtAddress);
      printf("Length:\t\t\t%u\n", length);
      printf("XSDT Address:\t\t0x%llx\n", xsdtAddress);
      printf("XChecksum:\t\t0x%x\n", extendedChecksum);
   }

   uint8_t calculateChecksum()
   {
      uint8_t sum = 0;

      std::size_t size = (std::size_t )((uint8_t *)&length - (uint8_t *)this);

      uint8_t* rsdp = (uint8_t *)this;
      for (unsigned int i = 0; i < size; i++)
      {
	 sum += rsdp[i];
      }

      return sum;
   }

   uint8_t calculateExtendedChecksum()
   {
      uint8_t sum = 0;

      uint8_t* rsdp = (uint8_t *)this;
      for (unsigned int i = 0; i < sizeof(*this); i++)
      {
	 sum += rsdp[i];
      }

      return sum;
   }
} __attribute__((packed));

class Acpi
{
public:
   static Rsdp* findRsdp(char*, char*);
   static void printAllDescriptors();
};

#endif
