#include <X86/Idt.hh>
#include <X86/Processor.hh>
#include <X86/ThreadContext.hh>

#include <Memory.hh>
#include <Interrupt.hh>
#include <Power.hh>
#include <Debug.hh>
#include <StackTrace.hh>
#include <Scheduler.hh>
#include <Thread.hh>

extern IdtPointer idtPointer;

// Idt descriptors
static IdtEntry idtEntries[NUMIDT];
IdtPointer idtPointer;

extern "C" InterruptFrame* x86_isr_dispatcher(InterruptFrame* frame);
extern "C" void x86_isr_init(int n, void (*handler)());

#include "InterruptVectorsInit.icc"

// XXX this should be refactored to be MI
//
static void
x86_isr_page_fault(InterruptFrame* frame)
{
   static int nestingFlag;

   // protect the kernel from trying to handle nested page faults
   //
   if (nestingFlag == 1)
   {
      Debug::panic("Nested page fault");
   } else if (nestingFlag == 2) {
      printf("Nested page fault, panic failed!\n");
      Power::reboot();
   } else if (nestingFlag > 2) {
      Power::reboot();
   }

   nestingFlag++;

   uint32_t cr2 = get_cr2();

   if ((frame->error & 0x1) == 0 && Memory::handlePageFault(cr2, frame))
   {
#ifdef DEBUG
      printf("Page fault handled: %p\n", (void* )cr2);
#endif
      nestingFlag--;
      return;
   }

   printf("\n");
   frame->print();

   printf("\nPage fault: %s\n", (frame->error & (1 << 0)) ? "protection violation" : "page not present");
   printf("%s 0x%x from 0x%x in %s mode\nError Code: 0x%x\n",
	  (frame->error & (1 << 1)) ? "Writing" : "Reading",
	  cr2, frame->eip,
	  (frame->error & (1 << 2)) ? "user" : "supervisor",
	  frame->error);

   StackTrace::printStackTrace(reinterpret_cast<void*>(frame->ebp));

   cli();
   Power::halt();
}

static void
x86_isr_general_protection_fault(InterruptFrame* frame)
{
   printf("\n");
   frame->print();

   printf("\nGeneral protection fault: ");

   switch ((frame->error >> 1) & 0x3)
   {
      case 0:
	 printf("GDT");
	 break;
      case 2:
	 printf("LDT");
	 break;
      case 1:
      case 3:
	 printf("IDT");
	 break;
   }

   printf(": 0x%x", (frame->error >> 3) & 0x1fff);

   if (frame->error & 0x1)
   {
      printf(" (external)");
   }

   printf("\n\n");

   StackTrace::printStackTrace(reinterpret_cast<void*>(frame->ebp));

   cli();
   Power::halt();
}

static void
x86_isr_default_handler(InterruptFrame* frame)
{
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
}

InterruptFrame*
x86_isr_dispatcher(InterruptFrame* frame)
{
   KASSERT(Interrupt::getInterruptLevel() > 0);

   if (frame->interrupt == 32)
   {
      Thread* currentThread = Scheduler::getCurrentThread();
      currentThread->kernelStack = (uintptr_t )frame;
   }

   // TODO: make a list of handlers and register them there to be run from dispatcher
   if (frame->interrupt == 13)
   {
      x86_isr_general_protection_fault(frame);
   }

   if (frame->interrupt == 14)
   {
      x86_isr_page_fault(frame);
      goto exit;
   }

   // XXX hardcoded hack
   if (frame->interrupt >= 32 && frame->interrupt < 48)
   {
      Interrupt::handleInterrupt(frame->interrupt - 32);
      goto exit;
   }

   x86_isr_default_handler(frame);

  exit:
   KASSERT(Interrupt::getInterruptLevel() > 0);

   if (frame->interrupt == 32)
   {
      Thread* currentThread = Scheduler::getCurrentThread();
      InterruptFrame* newFrame = (InterruptFrame* )currentThread->kernelStack;

      KASSERT(newFrame != 0);

      frame = newFrame;
   }

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

   x86_idt_init();
}
