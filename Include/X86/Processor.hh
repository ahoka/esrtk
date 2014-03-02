#ifndef ASSEMBLY_HH
#define ASSEMBLY_HH

#include <stdint.h>

struct cpuid_t {
   uint32_t eax;
   uint32_t ebx;
   uint32_t ecx;
   uint32_t edx;
} __attribute__((packed));

enum Flags
{
   Carry		= (1 << 0),
   Parity		= (1 << 2),
   Adjust		= (1 << 4),
   Zero			= (1 << 6),
   Sign			= (1 << 7),
   Trap			= (1 << 8),
   InterruptEnable	= (1 << 9),
   Direction		= (1 << 10),
   Overflow		= (1 << 11)
};

#if 0
struct Flags
{
   unsigned CarryFlag			: 1;
   unsigned _unused1_			: 1;
   unsigned ParityFlag			: 1;
   unsigned _unused2_			: 1;
   unsigned AdjustFlag			: 1;
   unsigned _unused3_			: 1;
   unsigned ZeroFlag			: 1;
   unsigned SignFlag			: 1;
   unsigned TrapFlag			: 1;
   unsigned InterruptEnableFlag		: 1;
   unsigned DirectionFlag		: 1;
   unsigned OverflowFlag		: 1;
   unsigned IoPrivilegeLevel		: 2;
   unsigned NestedTask			: 1;
   unsigned _reserved4_			: 1;
   unsigned ResumeFlag			: 1;
   unsigned Virtual8086Flag		: 1;
   unsigned AlignmentCheck		: 1;
   unsigned VirtualInterruptFlag	: 1;
   unsigned VirtualInterruptPending	: 1;
   unsigned IdFlag			: 1;
   unsigned _reserved5_			: 10;
};

static_assert(sizeof(Flags) == 32, "struct Flags should be 32 bits in size");
#endif

extern "C" void cli();
extern "C" void sti();

extern "C" void wrmsr(uint32_t address, uint32_t eax, uint32_t edx);
extern "C" void rdmsr(uint32_t address, uint32_t* eax, uint32_t* edx);

extern "C" uint64_t rdtsc();

extern "C" void cpuid(uint32_t eax, cpuid_t* result);
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
