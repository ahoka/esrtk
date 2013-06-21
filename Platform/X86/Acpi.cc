#include <Acpi.hh>

Rsdt *
Acpi::findRsdt()
{
   for (char* mem = (char*)0x0e0000; mem < (char*)0x0fffff; mem += 16)
   {
      if (mem[0] == 'R' && mem[1] == 'S' && mem[2] == 'D' &&
          mem[3] == ' ' && mem[4] == 'P' && mem[5] == 'T' &&
          mem[6] == 'R' && mem[7] == ' ')
      {  
         Rsdt* rsdt = (Rsdt* )mem;

	 return rsdt;
      }
   }

   return 0;
}
