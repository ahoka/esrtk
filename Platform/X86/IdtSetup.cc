#include <Idt.hh>
#include <stdio.h>

#define NUMIDT 256

// Idt descriptors
IdtEntry idtEntries[NUMIDT];
IdtPointer idtPointer;

extern "C" void idtinit();
extern "C" void isr0();

extern "C" void
defaultIsr()
{
   printf("UNHANDLED INTERRUPT!\n");
   for(;;)
   {
      volatile int b = 1;
   }
}

extern "C" void
isrDispatcher(void *)
{
//   defaultIsr();
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
   idtPointer.limit = sizeof(idtEntries - 1);

   for (int i = 0; i < NUMIDT; i++)
   {
      initIsr(i, isr0);
   }
}
