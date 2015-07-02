#include <Multiboot.hh>
#include <Parameters.hh>
#include <Debug.hh>

// XXX BSD compat leaking int the kernel!
namespace elf
{
#include <sys/types.h>
#include <sys/elf32.h>
}

#include <cstring>

using Multiboot::MultibootStructure;

static MultibootStructure mbCopy;

static char mbCommandLine[BootCommandLineMax];
static char mbBootloaderName[32];

extern MultibootStructure* mbd;

extern "C" void
copy_multiboot_structure()
{
   memcpy(&mbCopy, mbd, sizeof(mbCopy));

   if (mbd->flags & MultibootStructure::CommandLineValid)
   {
      strlcpy(mbCommandLine, mbd->commandLine, sizeof(mbCommandLine));
      mbCopy.commandLine = mbCommandLine;
   }

   if (mbd->flags & MultibootStructure::BootLoaderNameValid)
   {
      strlcpy(mbBootloaderName, mbd->bootLoaderName, sizeof(mbBootloaderName));
      mbCopy.bootLoaderName = mbBootloaderName;
   }
}

const MultibootStructure*
Multiboot::getMultibootStructure()
{
   return &mbCopy;
}

void
Multiboot::getSymbols()
{
   printf("Populating kernel symbol table\n");

   auto m = getMultibootStructure();
   if ((m->flags & MultibootStructure::ElfHeaderValid))
   {
      auto elf = &m->symbol.elf;
      printf("ELF Header valid: %u\n", elf->number);

#if 0
      // xxx map this for access
      for (int i = 0; i < elf->number; i++)
      {
         auto section = (elf::Elf32_Shdr*)(elf->address + elf->size * i);

         printf("Type: %u\n", section->sh_type);
         printf("Address: %p\n", (void*)section->sh_addr);
         printf("Size: %u\n", section->sh_size);
      }
#endif
   }
}
