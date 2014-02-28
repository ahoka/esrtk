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

extern "C" void idtinit();

extern "C" void idtinit();
// extern "C" void isr0();
// extern "C" void isr1();
// extern "C" void isr2();
// extern "C" void isr3();
// extern "C" void isr4();
// extern "C" void isr5();
// extern "C" void isr6();
// extern "C" void isr7();
// extern "C" void isr8();
// extern "C" void isr9();
// extern "C" void isr10();
// extern "C" void isr11();
// extern "C" void isr12();
// extern "C" void isr13();
// extern "C" void isr14();
// extern "C" void isr16();

void initInterruptVectors();
extern "C" void initInterrupts();

#endif
