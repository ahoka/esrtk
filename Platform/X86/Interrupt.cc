#include <X86/Idt.hh>
#include <X86/Assembly.hh>
#include <Memory.hh>

#include <Interrupt.hh>
#include <Power.hh>
#include <Debug.hh>

#include <StackTrace.hh>

extern IdtPointer idtPointer;

// Idt descriptors
static IdtEntry idtEntries[NUMIDT];
IdtPointer idtPointer;

extern "C" InterruptFrame* x86_isr_dispatcher(InterruptFrame* frame);
extern "C" void x86_isr_init(int n, void (*handler)());

#include "InterruptVectorsInit.icc"

static void
pageFault(InterruptFrame* frame)
{
   uint32_t cr2 = getCr2();

   if ((frame->error & 0x1) == 0 && Memory::handlePageFault(cr2, frame))
   {
#ifdef DEBUG
      printf("Page fault handled: %p\n", (void* )cr2);
#endif
      return;
   }

   printf("\n");
   frame->print();

   printf("\nPage fault: %s\n", (frame->error & (1 << 0)) ? "protection violation" : "page not present");
   printf("%s 0x%x from 0x%x in %s mode\nError Code: 0x%x\n",
	  (frame->error & (1 << 1)) ? "Writing" : "Reading",
	  cr2, frame->rip,
	  (frame->error & (1 << 2)) ? "user" : "supervisor",
	  frame->error);

   StackTrace::printStackTrace(reinterpret_cast<void*>(frame->ebp));

   Power::halt();
}

static void
defaultIsr(InterruptFrame* frame)
{
   // TODO: make a list of handlers and register them there to be run from dispatcher
   if (frame->interrupt == 14)
   {
      pageFault(frame);
      return;
   }

   if (frame->error == 0)
   {
      printf("\n");
      frame->print();

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

InterruptFrame*
x86_isr_dispatcher(InterruptFrame* frame)
{
   KASSERT(Interrupt::getInterruptLevel() > 0);

   // XXX hardcoded hack
   if (frame->interrupt >= 32 && frame->interrupt < 48)
   {
      Interrupt::handleInterrupt(frame->interrupt - 32);
      return frame;
   }

   defaultIsr(frame);

   KASSERT(Interrupt::getInterruptLevel() > 0);

   return frame;
}

void
x86_isr_init(int n, void (*handler)())
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
