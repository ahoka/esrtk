#include <X86/Processor.hh>
#include <cstdint>

const uint32_t IA32_PERFEVTSEL0 = 0x186;
const uint32_t IA32_PERFEVTSEL1 = 0x187;
const uint32_t IA32_PERFEVTSEL2 = 0x188;
const uint32_t IA32_PERFEVTSEL3 = 0x189;

class PerfCounter
{
public:
   bool init();
};

bool
PerfCounter::init()
{
   cpuid id;
   x86_cpuid(0x0a, &id);
   
   return true;
}
