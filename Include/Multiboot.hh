#ifndef MULTIBOOT_HH
#define MULTIBOOT_HH

#include <stdint.h>

struct Multiboot
{
   uint32_t flags;
   uint32_t memLower;
   uint32_t memUpper;
   uint32_t bootDevice;
   const char* cmdline;
   // ...
} __attribute__((packed));

#endif
