#ifndef _IDT_H_
#define _IDT_H_

#include <stdint.h>

struct idtEntry
{
   uint16_t baseLow;
   uint16_t selector;
   uint8_t reserved;
   uint8_t flags;
   uint16_t baseHigh;
} __attribute__((packed));

extern "C" void idtinit();

#endif
