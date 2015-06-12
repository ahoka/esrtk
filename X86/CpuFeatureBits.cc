#include <X86/CpuRegisters.hh>
#include <X86/Processor.hh>

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

void
printCpuFeatures()
{
   struct cpuid id;

   cpuid(1, &id);

   if (id.edx & CPUID::FPU)
   {
      printf("fpu ");
   }
   if (id.edx & CPUID::MSR)
   {
      printf("msr ");
   }

   if (id.edx & CPUID::APIC)
   {
      printf("apic ");
   }

   if (id.edx & 1 << 30)
   {
      printf("ia64 ");
   }

   printf("\n");

}
