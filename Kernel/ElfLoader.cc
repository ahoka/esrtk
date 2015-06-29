#include <ElfLoader.hh>
#include <gelf.h>

#include <cstdio>

ElfLoader::ElfLoader()
{
}

ElfLoader::~ElfLoader()
{
}

ElfLoader&
ElfLoader::loader()
{
   static ElfLoader loader;
   return loader;
}

static void
loadElf(Elf* elf)
{
   char* ident = elf_getident(elf, NULL);
   if (ident != NULL)
   {
      switch (ident[EI_CLASS])
      {
         case ELFCLASS32:
            printf(", 32bit");
            break;
         case ELFCLASS64:
            printf(", 64bit");
            break;
      }
      switch (ident[EI_DATA])
      {
         case ELFDATA2LSB:
            printf(", little-endian");
            break;
         case ELFDATA2MSB:
            printf(", big-endian");
            break;
      }
   }
}

void
printSections(Elf* elf)
{
   Elf_Scn *section;
   GElf_Shdr sectionHeader;
   size_t stringSectionIndex;

   if (elf_getshdrstrndx(elf, &stringSectionIndex) != 0)
   {
      printf("elf_getshdrstrndx error: %s\n", elf_errmsg(-1));
      return;
   }

   section = 0;
   while ((section = elf_nextscn(elf, section)) != 0)
   {
      if (gelf_getshdr(section, &sectionHeader) != &sectionHeader)
      {
         printf("gelf_getshdr error: %s\n", elf_errmsg(-1));
         return;
      }

      char* name = elf_strptr(elf, stringSectionIndex, sectionHeader.sh_name);
      if (name == 0)
      {
         printf("elf_strptr error: %s\n", elf_errmsg(-1));
         return;         
      }

      printf("  %s\n", name);
   }
}

void
ElfLoader::load(uintptr_t address, size_t size)
{
   Elf* elf;

   (void)elf_version(EV_CURRENT);

   elf = elf_memory((char*)address, size);
   printf("ELF loaded: %p\n", elf);

   printf("Module type is: ");

   Elf_Kind ek = elf_kind(elf);
   switch (ek)
   {
      case ELF_K_ELF:
         printf("ELF");
         loadElf(elf);
         printf("\n");
         printSections(elf);
         break;
      case ELF_K_AR:
         printf("AR\n");
         break;
      case ELF_K_NONE:
         printf("Data\n");
         break;
      default:
         printf("Unknown\n");
   }
   
   elf_end(elf);
}

