#include <Idt.hh>
#include <stdio.h>
#include <Debug.hh>

// Idt descriptors
IdtEntry idtEntries[NUMIDT];
IdtPointer idtPointer;

void initIsr(int n, void (*handler)());

#include "InterruptVectorsInit.icc"

extern "C" void
defaultIsr(InterruptFrame* frame)
{
   if (frame->error)
   {
      Debug::panic("\n\nUnhandled Interrupt: %u\n", frame->interrupt);
   }
   else
   {
      Debug::panic("\n\nUnhandled Interrupt: %u, Error Code: 0x%x\n", frame->interrupt, frame->error);
   }

   // printf("\nInterrupt frame:\n");
   // frame->print();
   // printf("\n");
}

extern "C" void
isrDispatcher(InterruptFrame* frame)
{
   defaultIsr(frame);
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

   // generated function
   initInterruptVectors();

   idtinit();
}
