#include <X86/Assembly.hh>
#include <cstdio>

// struct Features
// {
//    const char* name;
//    bool present;
// };

// Features edx_1[32] = {
//    { "fpu", false },
//    { "vme", false },
//    { "fpu", false },
//    { "fpu", false },
// };

enum
{
   fpu = 1 << 0,
   vme = 1 << 1,
   de = 1 << 2,
   pse = 1 << 3,
   tsc = 1 << 4,
   msr = 1 << 5,
   pae = 1 << 6,
   mce = 1 << 7,
   cx8 = 1 << 8,
   apic = 1 << 9
};

extern "C" void
printCpuFeatures()
{
   cpuid_t id;

   cpuid(1, &id);

   if (id.edx & fpu)
   {
      printf("fpu ");
   }
   if (id.edx & msr)
   {
      printf("msr ");
   }

   if (id.edx & apic)
   {
      printf("apic ");
   }

   if (id.edx & 1 << 30)
   {
      printf("ia64 ");
   }

   printf("\n");

}
