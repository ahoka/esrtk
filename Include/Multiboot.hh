#ifndef MULTIBOOT_HH
#define MULTIBOOT_HH

#include <stdint.h>
#include <stdio.h>

struct Multiboot
{
   uint32_t flags;
   uint32_t lowMemory;
   uint32_t highMemory;
   uint32_t bootDevice;
   const char* commandLine;
   uint32_t modulesCount;
   uint32_t modulesAddress;
   union Symbol
   {
      struct Aout
      {
   	 uint32_t tabSize;
   	 uint32_t stringSize;
   	 uint32_t address;
   	 uint32_t _reserved;
      } __attribute__((packed)) aout;
      struct Elf
      {
	 uint32_t number;
	 uint32_t size;
	 uint32_t address;
	 uint32_t shndx; // ???
      } __attribute__((packed)) elf;
   } symbol;
   uint32_t memoryMapLength;
   uint32_t memoryMapAddress;
   uint32_t drivesLength;
   uint32_t drivesAddress;
   uint32_t configurationTable;
   const char* bootLoaderName;
   uint32_t apmTable;
   uint32_t vbeControlInfo;
   uint32_t vbeModeInfo;
   uint32_t vbeMode;
   uint32_t vbeInterfaceSegment;
   uint32_t vbeInterfaceOffset;
   uint32_t vbeInterfaceLength;

   struct MemoryMap
   {
      uint32_t size;
      uint64_t address;
      uint64_t length;
      uint32_t type;

      enum MemoryType
      {
	 Available = 1,
	 Reserved,
	 Acpi,
	 Nvs
      };
   };

   enum
   {
      MemoryValid = (1 << 0),
      BootDeviceValid = (1 << 1),
      CommandLineValid = (1 << 2),
      ModulesValid = (1 << 3),
      AoutSymbolsValid = (1 << 4),
      ElfHeaderValid = (1 << 5),
      MemoryMapValid = (1 << 6),
      DrivesValid = (1 << 7),
      ConfigTableValid = (1 << 8),
      BootLoaderNameValid = (1 << 9),
      ApmTableValid = (1 << 10),
      VbeValid = (1 << 11)
   };

   inline void printField(const char* name, uint32_t value)
   {
      printf("%s: 0x%x (%u)\n", name, value, value);
   }

   // inline void printField(const char* name, uint64_t value)
   // {
   //    printf("%s: 0x%llx (%llu)\n", name, value, value);
   // }

   inline void printField(const char* name, const char* value)
   {
      printf("%s: %s\n", name, value);
   }

   inline void printField(MemoryMap* mmap)
   {
      const char* typeName;
      switch (mmap->type)
      {
	 case MemoryMap::Available:
	    typeName = "Available";
	    break;
	 case MemoryMap::Reserved:
	    typeName = "Reserved";
	    break;
	 case MemoryMap::Acpi:
	    typeName = "ACPI";
	    break;
	 case MemoryMap::Nvs:
	    typeName = "NVS";
	    break;
	 default:
	    typeName = "Unknown";
      }

      printf("Address: 0x%x-0x%x (%u bytes), %s\n",
	     (uint32_t)mmap->address, (uint32_t)(mmap->address + mmap->length),
             (uint32_t)mmap->length, typeName);
   }

   void print()
   {
      printf("Multiboot header:\n");

      if (flags & MemoryValid)
      {
	 printField("Low Memory", lowMemory);
	 printField("High Memory", highMemory);
      }

      if (flags & BootDeviceValid)
      {
	 printField("Boot Device", bootDevice);
      }

      if (flags & CommandLineValid)
      {
	 printField("Command Line", commandLine);
      }

      if (flags & ModulesValid)
      {
	 printField("Modules Count", modulesCount);
	 printField("Modules Address", modulesAddress);
      }

      if (flags & AoutSymbolsValid)
      {
	 printf("Symbols: A.OUT\n");
      }

      if (flags & ElfHeaderValid)
      {
	 printf("Symbols: Elf\n");
      }

      if (flags & MemoryMapValid)
      {
	 printField("Memory Map Length:", memoryMapLength);
	 printField("Memory Map Address:", memoryMapAddress);
	 for (MemoryMap *map = (MemoryMap *)memoryMapAddress;
	      map < (MemoryMap *)(memoryMapAddress + memoryMapLength);
	      map++)
	 {
	    printField(map);
	 }
      }

      if (flags & DrivesValid)
      {
	 printField("Drives Length:", drivesLength);
	 printField("Drives Address:", drivesAddress);
      }

      if (flags & ConfigTableValid)
      {
	 printField("Configuration Table", configurationTable);
      }

      if (flags & BootLoaderNameValid)
      {
	 printField("Boot Loader Name", bootLoaderName);
      }

      if (flags & ApmTableValid)
      {
	 printField("APM Table", apmTable);
      }

      if (flags & VbeValid)
      {
	 printField("VBE Control Info", vbeControlInfo);
	 printField("VBE Mode Info", vbeModeInfo);
	 printField("VBE Mode", vbeMode);
	 printField("VBE Interface Segment", vbeInterfaceSegment);
	 printField("VBE Interface Offset", vbeInterfaceOffset);
	 printField("VBE Interface Length", vbeInterfaceLength);
      }
   }
} __attribute__((packed));

#endif
