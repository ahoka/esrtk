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

#endif
