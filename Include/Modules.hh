#ifndef MODULES_HH
#define MODULES_HH

#include <cstddef>
#include <cstdint>
#include <Parameters.hh>

extern uintptr_t __start_modules;
extern uintptr_t __end_modules;

class Modules
{
public:
   struct Module
   {
      uintptr_t address;
      std::size_t size;
      char name[PathMax];
   };

   struct ModuleHeader
   {
      uint32_t magic;
      uint32_t type;
      uint32_t length;
      uint32_t checksum;
   } __attribute__((packed));

   enum
   {
      Magic = 0x30646f4d
   };

   enum Type
   {
      Ramdisk = 0,
      NumberOfTypes
   };

   static const char* typeToTypename[NumberOfTypes];

   static void init();
   static void add(uintptr_t start, std::size_t size, const char* name);
   static void handleModules();

private:
   Modules();
   Modules(const Modules& orig);

   static const unsigned int ModulesMax = 32;

   static Module list[ModulesMax];
   static unsigned int nextFree;
};

#endif
