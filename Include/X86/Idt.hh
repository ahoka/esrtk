#ifndef _IDT_H_
#define _IDT_H_

#include <cstdint>
#include <cstdio>

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

struct InterruptFrame
{
   uint32_t gs;
   uint32_t fs;
   uint32_t es;
   uint32_t ds;
   uint32_t edi;
   uint32_t esi;
   uint32_t ebp;
   uint32_t esp;
   uint32_t ebx;
   uint32_t edx;
   uint32_t ecx;
   uint32_t eax;
   uint32_t interrupt;
   uint32_t error;
   uint32_t rip;
   uint32_t cs;
   uint32_t eflags;
   uint32_t useresp;
   uint32_t ss;

   void print()
   {
      printf("edi: 0x%0x esi: 0x%0x ebp: 0x%0x esp: 0x%0x\n", edi, esi, ebp, esp);
      printf("ebx: 0x%0x edx: 0x%0x ecx: 0x%0x eax: 0x%0x\n", ebx, edx, ecx, eax);
      printf("gs: 0x%0x fs: 0x%0x es: 0x%0x ds: 0x%0x\n",
             gs & 0xffff, fs & 0xffff, es & 0xffff, ds & 0xffff);
      printf("interrupt id: %u error code: 0x%0x flags: 0x%0x\n", interrupt, error, eflags);
   }
} __attribute__((packed));

extern "C" void idtinit();
extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr16();

void initInterruptVectors();
extern "C" void initInterrupts();

#endif
