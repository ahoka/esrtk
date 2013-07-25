#include <X86/Idt.hh>
#include <X86/Assembly.hh>

#include <Debug.hh>
#include <Power.hh>

#include <cstdio>

extern "C" void defaultIsr(InterruptFrame* frame) __attribute__((noreturn));
extern "C" void isrDispatcher(InterruptFrame* frame) __attribute__((noreturn));

extern IdtPointer idtPointer;

// Idt descriptors
static IdtEntry idtEntries[NUMIDT];
IdtPointer idtPointer;

void initIsr(int n, void (*handler)());

#include "InterruptVectorsInit.icc"

void
defaultIsr(InterruptFrame* frame)
{
   if (frame->interrupt == 14)
   {
      uint32_t cr2 = getCr2();
      printf("\n\nPage fault: %s\n", (frame->error & (1 << 0)) ? "protection violation" : "page not present");
      printf("%s 0x%x from 0x%x in %s mode\nError Code: 0x%x\n",
             (frame->error & (1 << 1)) ? "Writing" : "Reading",
             cr2, frame->rip,
             (frame->error & (1 << 2)) ? "user" : "supervisor",
             frame->error);

      Power::halt();
   }

   if (frame->error == 0)
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

void
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

void
initInterrupts()
{
   idtPointer.base = (uint32_t )&idtEntries;
   idtPointer.limit = sizeof(IdtEntry) * NUMIDT - 1;

   // generated function
   initInterruptVectors();

   idtinit();
}
