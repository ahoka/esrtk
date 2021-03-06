#ifndef THREADCONTEXT_HH
#define THREADCONTEXT_HH

#include <cstdio>
#include <cstdint>

#include <Kernel/Thread.hh>

struct InterruptFrame
{
   uint32_t gs;
   uint32_t fs;
   uint32_t es;
   uint32_t ds;
   uint32_t edi;
   uint32_t esi;
   uint32_t ebp;
   uint32_t edx;
   uint32_t ecx;
   uint32_t ebx;
   uint32_t eax;
   uint32_t interrupt;
   uint32_t error;
   uint32_t eip;
   uint32_t cs;
   uint32_t eflags;
   uint32_t esp;
   uint32_t ss;

   void print()
   {
      printf("Frame: %p\n", this);
      printf("edi: 0x%0x esi: 0x%0x ebp: 0x%0x esp: 0x%0x\n", edi, esi, ebp, esp);
      printf("ebx: 0x%0x edx: 0x%0x ecx: 0x%0x eax: 0x%0x\n", ebx, edx, ecx, eax);
      printf("gs:  0x%0x fs:  0x%0x es:  0x%0x ds:  0x%0x\n",
             gs & 0xffff, fs & 0xffff, es & 0xffff, ds & 0xffff);
      printf("cs:  0x%0x ss:  0x%0x eip: 0x%0x\n", cs & 0xffff, ss & 0xffff, eip);
      printf("interrupt id: %u error code: 0x%0x flags: 0x%0x\n", interrupt, error, eflags);
   }
} __attribute__((packed));

namespace ThreadContext
{
   uintptr_t initKernelStack(uintptr_t top, uintptr_t main, uintptr_t arg);
   uintptr_t initUserStack(uintptr_t top, uintptr_t main, uintptr_t arg);
};

#endif
