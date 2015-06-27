#include <X86/Idt.hh>
#include <X86/Processor.hh>
#include <X86/ThreadContext.hh>

#include <Memory.hh>
#include <Interrupt.hh>
#include <Power.hh>
#include <Debug.hh>
#include <StackTrace.hh>
#include <Kernel/Scheduler.hh>
#include <Kernel/Thread.hh>
#include <Kernel/Process.hh>
#include <Kernel/ProcessContext.hh>
#include <Kernel/SystemCall.hh>

using namespace Kernel;

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
   if (nestingFlag > 0)
   {
      Debug::panic("Nested page fault");
      Power::reboot();
   }

   nestingFlag++;

   uint32_t cr2 = x86_get_cr2();

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

   bool isUser = frame->error & (1 << 2);
   if (isUser)
   {
      Process* p = Scheduler::getCurrentProcess();
      KASSERT(p != Scheduler::getKernelProcess());
      printf("Terminating process %p\n", p);
      delete p;
      Scheduler::schedule(); // XXX
      nestingFlag--;
      return;
   }

   x86_cli();
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

   x86_cli();
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
   if (frame->interrupt == 255)
   {
      // spurious
      return frame;
   }

   KASSERT(Interrupt::getInterruptLevel() > 0);

   Thread* interruptedThread = Scheduler::getCurrentThread();
   Process* interruptedProcess = interruptedThread->getProcess();
   KASSERT(interruptedProcess == Scheduler::getCurrentProcess());
   interruptedThread->setKernelStack((uintptr_t )frame);
   interruptedThread->setReady();

   // TODO: make a list of handlers and register them there to be run from dispatcher
   if (frame->interrupt == 13)
   {
      x86_isr_general_protection_fault(frame);
   }
   else if (frame->interrupt == 14)
   {
      x86_isr_page_fault(frame);
   }
   else if (frame->interrupt == 128)
   {
      int rv = handleSystemCall(frame->eax, (void*)frame->esp);
      frame->eax = rv;
   }
   else if (frame->interrupt >= 32 && frame->interrupt < 48)
   {
      Interrupt::handleInterrupt(frame->interrupt - 32);
   }
   else
   {
      x86_isr_default_handler(frame);
   }

   KASSERT(Interrupt::getInterruptLevel() > 0);

   Thread* currentThread = Scheduler::getCurrentThread();
   Process* currentProcess = currentThread->getProcess();
   if (currentThread->getType() == Thread::UserThread)
   {
      ProcessContext::setKernelStack(currentThread->getKernelStack());
   }

   currentThread->setRunning();

   if (currentProcess != interruptedProcess)
   {
      KASSERT(currentProcess != 0);
      ProcessContext* newContext = currentProcess->getContext();
      KASSERT(newContext != 0);
      printf("switching processcontext: (%p) %p\n", currentProcess, newContext);
      newContext->switchContext();
   }
   InterruptFrame* newFrame = (InterruptFrame* )currentThread->getKernelStack();

   KASSERT(newFrame != 0);

   frame = newFrame;

   return frame;
}

void
x86_isr_init(int n, void (*handler)())
{
   idtEntries[n].baseLow = (uint16_t)((uint32_t)(handler) & 0xffff);
   idtEntries[n].baseHigh = (uint16_t)(((uint32_t)(handler) >> 16) & 0xffff);

   idtEntries[n].selector = 0x08;
   idtEntries[n].gateType = 0xe; // XXX InterruptGate
   idtEntries[n].storageSegment = 0;
   idtEntries[n].dpl = 0;
   idtEntries[n].present = 1;

   // zero filled
   idtEntries[n].reserved = 0;

   // XXX should be done properly
   // mark 0x80 as user callable for syscalls
   if (n == 0x80)
   {
      idtEntries[n].dpl = 3;
   }
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
