#include <Kernel/PerfCounter.hh>
#include <X86/Processor.hh>

#include <cstdint>
#include <cstdio>

const uint32_t IA32_PERFEVTSEL0 = 0x186;
const uint32_t IA32_PERFEVTSEL1 = 0x187;
const uint32_t IA32_PERFEVTSEL2 = 0x188;
const uint32_t IA32_PERFEVTSEL3 = 0x189;

const uint32_t IA32_PMC0 = 0x0c1;
const uint32_t IA32_PMC1 = 0x0c2;
const uint32_t IA32_PMC2 = 0x0c3;
const uint32_t IA32_PMC3 = 0x0c4;

PerfCounter instance;

PerfCounter::PerfCounter()
{
   if (probe() > 0)
   {
      init();
   }
}

int
PerfCounter::probe()
{
   printf("Probing for Performance counters\n");

   cpuid id;
   x86_cpuid(0x0a, &id);
   
   uint32_t present = id.eax >> 8 & 0xff;
   if (present > 0)
   {
      printf(" Processor has IA32_PERFEVTSEL0\n");

      uint64_t perf = x86_rdmsr(IA32_PERFEVTSEL0);
      uint64_t pmc = x86_rdmsr(IA32_PMC0);

      printf("  Event select: 0x%x\n", (int)(perf & 0xff));
      printf("  UMask:        0x%x\n", (int)((perf >> 8) & 0xff));
      printf("  INT:          0x%x\n", (int)((perf >> 20) & 0x01));
      printf("  EN:           0x%x\n", (int)((perf >> 22) & 0x01));

      printf("  PMC:          0x%llx\n", pmc);
   }
   else
   {
      printf(" Processor has no performance counters.\n");
   }

   if (present > 1)
   {
      printf(" Processor has IA32_PERFEVTSEL1\n");
   }

   if (present > 2)
   {
      printf(" Processor has IA32_PERFEVTSEL2\n");
   }

   if (present > 3)
   {
      printf(" Processor has IA32_PERFEVTSEL3\n");
   }

   return present > 0 ? 1 : 0;
}

bool
PerfCounter::init()
{
   
   return true;
}
