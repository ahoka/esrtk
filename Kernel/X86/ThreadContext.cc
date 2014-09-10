#include <Hal.hh>

#include <X86/ThreadContext.hh>
#include <X86/Processor.hh>

#include <Supervisor/Thread.hh>
#include <Parameters.hh>

#include <cstdint>

uintptr_t
Hal::initKernelStack(uintptr_t top)
{
   uintptr_t stack = top - sizeof(InterruptFrame);
   InterruptFrame* context = (InterruptFrame*)stack;

   context->gs = KernelDataSegment;
   context->fs = KernelDataSegment;
   context->es = KernelDataSegment;
   context->ds = KernelDataSegment;
   context->edi = 0x0;
   context->esi = 0x0;
   context->ebp = 0x0;
   context->edx = 0x0;
   context->ecx = 0x0;
   context->ebx = 0x0;
   context->eax = 0x0;
   context->interrupt = 0;
   context->error = 0;
   context->eip = reinterpret_cast<uint32_t>(&Thread::main); // XXX make this configurable
   context->cs = KernelCodeSegment;
   context->eflags = Flags::Reserved | Flags::InterruptEnable;
   context->esp = stack;
   context->ss = KernelDataSegment;

   return stack;
}
