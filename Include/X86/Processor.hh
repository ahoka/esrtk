#ifndef ASSEMBLY_HH
#define ASSEMBLY_HH

#include <stdint.h>

struct cpuid {
   uint32_t eax;
   uint32_t ebx;
   uint32_t ecx;
   uint32_t edx;
} __attribute__((packed));

enum Flags
{
   Carry		= (1 << 0),
   Reserved		= (1 << 1),
   Parity		= (1 << 2),
   Adjust		= (1 << 4),
   Zero			= (1 << 6),
   Sign			= (1 << 7),
   Trap			= (1 << 8),
   InterruptEnable	= (1 << 9),
   Direction		= (1 << 10),
   Overflow		= (1 << 11)
};

// performance counters
namespace PerfEvtSel
{
   enum
   {
      EventSelectShift = 0,
      EventSelectMask = 0x0f,
      UnitMaskShift = 8,
      UnitMaskMask = 0x0f,
      CounterMaskShift = 24,
      CounterMaskMask = 0x0f,

      UserMode = (1 << 16),
      OperatingSystemMode = (1 << 17),
      EdgeDetect = (1 << 18),
      PinControl = (1 << 19),
      InterruptEnable = (1 << 20),
      EnableCounters = (1 << 22),
      InverCounterMask = (1 << 23)
   };
};

extern "C" void x86_cli();
extern "C" void x86_sti();

extern "C" void x86_wrmsr(uint32_t address, uint64_t value);
extern "C" uint64_t x86_rdmsr(uint32_t address);
extern "C" uint64_t x86_rdpmc(uint32_t counter);

extern "C" uint64_t rdtsc();

extern "C" void cpuid(uint32_t eax, cpuid* result);
extern "C" uint32_t cpuid0(char* result);

extern "C" uint32_t get_eflags();
extern "C" void set_eflags(uint32_t eflags);

extern "C" uint32_t get_cr2();

extern "C" void invlpg(uint32_t page);
extern "C" void set_cr3(uint32_t cr3);
extern "C" uint32_t get_cr3();
extern "C" void setCr0(uint32_t cr0);
extern "C" uint32_t get_cr0();
extern "C" void set_cr4(uint32_t cr4);
extern "C" uint32_t get_cr4();

extern "C" uintptr_t get_eip();

extern "C" void x86_init_paging(uint32_t cr3);

#endif
