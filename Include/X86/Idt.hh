#ifndef _IDT_H_
#define _IDT_H_

#include <cstdint>

#define NUMIDT 256

struct IdtEntry
{
   uint16_t baseLow;
   uint16_t selector;
   uint8_t reserved;
   uint8_t flags;
   uint16_t baseHigh;
} __attribute__((packed));

struct IdtPointer
{
   uint16_t limit;
   uint32_t base;
} __attribute__((packed));

extern "C" void x86_idt_init();
extern "C" void initInterrupts();
void initInterruptVectors();

#endif
