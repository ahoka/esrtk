#include <X86/Acpi.hh>
#include <cstdio>

Rsdp *
Acpi::findRsdp()
{
   for (char* mem = (char*)0x0e0000; mem < (char*)0x0fffff; mem += 16)
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
