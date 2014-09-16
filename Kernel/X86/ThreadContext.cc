#include <Hal.hh>

#include <X86/ThreadContext.hh>
#include <X86/Processor.hh>

#include <Kernel/Thread.hh>
#include <Parameters.hh>

#include <cstdint>

using namespace Kernel;

struct CallFrame
{
   uint32_t rip;
   uint32_t arg;
} __attribute__((packed));

uintptr_t
ThreadContext::initStack(uintptr_t top, uintptr_t main, uintptr_t arg)
{
   uintptr_t stack = top - sizeof(InterruptFrame) + 8 - sizeof(CallFrame);

   // create fake call frame for thread main to pass args
   //
   CallFrame* frame = reinterpret_cast<CallFrame*>(top - sizeof(CallFrame));
   frame->arg = arg;
   frame->rip = 0;

   printf("Args is at: %p\n", &frame->arg);

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
   context->eip = static_cast<uint32_t>(main);
   context->cs = KernelCodeSegment;
   context->eflags = Flags::Reserved | Flags::InterruptEnable;

   return stack;
}
