#include <X86/Memory.hh>
#include <X86/Parameters.hh>
#include <X86/Acpi.hh>
#include <cstdio>

Rsdp *
Acpi::findRsdp(char* from, char* to)
{
//   for (char* mem = (char*)0x0e0000; mem < (char*)0x0fffff; mem += 16)
   for (char* mem = from; mem < to; mem += 16)
   {
      if (mem[0] == 'R' && mem[1] == 'S' && mem[2] == 'D' &&
          mem[3] == ' ' && mem[4] == 'P' && mem[5] == 'T' &&
          mem[6] == 'R' && mem[7] == ' ')
      {  
         Rsdp* rsdp = (Rsdp* )mem;

	 if (rsdp->calculateChecksum() != 0)
	 {
	    printf("Found RSDP with invalid checksum!\n");
	 }
	 else
	 {
	    if (rsdp->calculateExtendedChecksum() == 0)
	    {
	       printf("Found ACPI 2.0+ RSDP at %p\n", rsdp);
	    }
	    else
	    {
	       printf("Found ACPI 1.0 RSDP at %p\n", rsdp);
	    }

	    return rsdp;
	 }
      }
   }

   return 0;
}

void
Acpi::printAllDescriptors()
{
   printf("looking for RSDP:\n");

   char* mem = (char* )Memory::mapRegion(0x0e0000, 0x20000);
   Rsdp* rsdp = Acpi::findRsdp(mem, mem + 0x20000);

//   if (rsdp->calculateExtendedChecksum() == 0)
//   {
      //Xsdt* xsdt = (Xsdt *)rsdp->xsdtAddress;
      Rsdt* rsdt = (Rsdt *)rsdp->rsdtAddress;
      Memory::unmapRegion(0x0e0000, 0x20000);

      uintptr_t rsdtPage = Memory::mapPage((uintptr_t)rsdt & ~PageMask);
      rsdt = (Rsdt* )(rsdtPage | ((uintptr_t)rsdt & PageMask));
      
      printf("RSDT at %p\n", (void*)rsdt);

      rsdt->printHeader();
      for (uint32_t i = 0;
           i < (rsdt->length - sizeof(DescriptionHeader)) / 4;
           i++)
      {
      	 ((DescriptionHeader *)rsdt->entry[i])->printHeader();
         printf("\n");
      }
//   }


}
