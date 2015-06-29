#ifndef ELFLOADER_HH
#define ELFLOADER_HH

#include <cstddef>
#include <cstdint>

class ElfLoader
{
public:
   static ElfLoader& loader();

   void load(uintptr_t address, size_t size);

private:
   ElfLoader();
   ~ElfLoader();
};

#endif
