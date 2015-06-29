#include <Multiboot.hh>
#include <Parameters.hh>
#include <Debug.hh>

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
   if ((m->flags & MultibootStructure::ElfHeaderValid) == 0)
   {
      printf("0x%08x\n", m->flags);
//      Debug::panic("Multiboot symbol header is invalid!");
   }
}
