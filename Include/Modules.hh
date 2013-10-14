#ifndef MODULES_HH
#define MODULES_HH

#include <cstddef>
#include <cstdint>
#include <Parameters.hh>

extern uintptr_t __end_modules;

class Modules
{
public:
   class Module
   {
   public:
      Module() :
	 address(0),
	 size(0)
      {
	 // empty
      }

      uintptr_t address;
      std::size_t size;
      char name[PathMax];
   };

   static void init();
   static void add(uintptr_t start, std::size_t size, const char* name);

private:
   Modules();
   Modules(const Modules& orig);

   static const unsigned int ModulesMax = 32;

   static Module list[ModulesMax];
   static unsigned int nextFree;
};

#endif
