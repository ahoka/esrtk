#ifndef __MSR_H__
#define __MSR_H__

#include <stdint.h>

typedef struct _cpuid {
  uint32_t eax;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t edx;
} __attribute__((packed)) cpuid_t;

#ifdef __cplusplus
extern "C" {
#endif

void wrmsr(uint32_t address, uint64_t value);
uint64_t rdmsr(uint32_t address);

void cpuid(uint32_t eax, void* result);

#ifdef __cplusplus
}
#endif

#endif
