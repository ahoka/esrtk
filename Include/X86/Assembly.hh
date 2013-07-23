#ifndef ASSEMBLY_HH
#define ASSEMBLY_HH

#include <stdint.h>

struct cpuid_t {
   uint32_t eax;
   uint32_t ebx;
   uint32_t ecx;
   uint32_t edx;
} __attribute__((packed));

extern "C" void wrmsr(uint32_t address, uint32_t eax, uint32_t edx);
extern "C" void rdmsr(uint32_t address, uint32_t* eax, uint32_t* edx);

extern "C" void cpuid(uint32_t eax, void* result);
extern "C" uint32_t cpuid0(char* result);

extern "C" uint32_t getEflags();
extern "C" void setEflags(uint32_t eflags);

extern "C" uint32_t getCr2();

extern "C" void invlpg(uint32_t page);
extern "C" void setCr3(uint32_t cr3);
extern "C" uint32_t getCr3();
extern "C" void setCr0(uint32_t cr0);
extern "C" uint32_t getCr0();
extern "C" void setCr4(uint32_t cr4);
extern "C" uint32_t getCr4();

#endif
