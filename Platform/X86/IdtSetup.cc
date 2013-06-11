#include <Idt.hh>
#include <stdio.h>
#include <Debug.hh>

// Idt descriptors
IdtEntry idtEntries[NUMIDT];
IdtPointer idtPointer;

extern "C" void
defaultIsr()
{
//   printf("UNHANDLED INTERRUPT!\n");
   Debug::panic("unhandled interrupt");
   // for (;;)
   // {
   //    asm volatile("pause");
   // }
}

extern "C" void
isrDispatcher(InterruptFrame* frame)
{
   frame->print();
   defaultIsr();
}

void
initIsr(int n, void (*handler)())
{
   idtEntries[n].baseLow = (uint16_t)((uint32_t)(handler) & 0xffff);
   idtEntries[n].baseHigh = (uint16_t)(((uint32_t)(handler) >> 16) & 0xffff);
   
   idtEntries[n].selector = 0x08;
   idtEntries[n].flags = 0x8e;
   
   // zero filled
   idtEntries[n].reserved = 0;
}

extern "C" void
initInterrupts()
{
   idtPointer.base = (uint32_t )&idtEntries;
   idtPointer.limit = sizeof(IdtEntry) * NUMIDT - 1;

   for (int i = 0; i < NUMIDT; i++)
   {
      initIsr(i, &defaultIsr);
   }

   initIsr(0, isr0);
   initIsr(1, isr1);
   initIsr(2, isr2);
   initIsr(3, isr3);
   initIsr(4, isr4);
   initIsr(5, isr5);
   initIsr(6, isr6);
   initIsr(7, isr7);
   initIsr(8, isr8);
   initIsr(9, isr9);
   initIsr(10, isr10);
   initIsr(11, isr11);
   initIsr(12, isr12);
   initIsr(13, isr13);
   initIsr(14, isr14);
   initIsr(16, isr16);

   idtinit();
}
