#ifndef __MSR_H__
#define __MSR_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void wrmsr(uint32_t address, uint64_t value);
uint64_t rdmsr(uint32_t address);

#ifdef __cplusplus
}
#endif

#endif
