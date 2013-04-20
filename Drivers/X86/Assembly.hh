#ifndef __MSR_H__
#define __MSR_H__

#include <stdint.h>

struct cpuid_t {
   uint32_t eax;
   uint32_t ebx;
   uint32_t ecx;
   uint32_t edx;
} __attribute__((packed));

extern "C" void wrmsr(uint32_t address, uint64_t value);
extern "C" uint64_t rdmsr(uint32_t address);

extern "C" void cpuid(uint32_t eax, void* result);
extern "C" uint32_t cpuid0(char* result);

extern "C" uint32_t getEflags();
extern "C" void setEflags(uint32_t eflags);

#endif