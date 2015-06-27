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
ThreadContext::initKernelStack(uintptr_t top, uintptr_t main, uintptr_t arg)
{
   uintptr_t stack = top - sizeof(InterruptFrame) + 8 - sizeof(CallFrame);

   // create fake call frame for thread main to pass args
   //
   CallFrame* frame = reinterpret_cast<CallFrame*>(top - sizeof(CallFrame));
   frame->arg = arg;
   frame->rip = 0;

   InterruptFrame* context = (InterruptFrame*)stack;

   context->gs = KernelDataSegment;
   context->fs = KernelDataSegment;
   context->es = KernelDataSegment;
   context->ds = KernelDataSegment;
   context->edi = 0x11111111;
   context->esi = 0x22222222;
   context->ebp = 0x0;
   context->edx = 0xdddddddd;
   context->ecx = 0xcccccccc;
   context->ebx = 0xbbbbbbbb;
   context->eax = 0xaaaaaaaa;
   context->interrupt = 0;
   context->error = 0;
   context->eip = static_cast<uint32_t>(main);
   context->cs = KernelCodeSegment;
   context->eflags = Flags::Reserved | Flags::InterruptEnable;

   return stack;
}

uintptr_t
ThreadContext::initUserStack(uintptr_t top, uintptr_t main, uintptr_t arg)
{
   uintptr_t stack = top - sizeof(InterruptFrame) - sizeof(CallFrame);

   // create fake call frame for thread main to pas args
   //
   CallFrame* frame = reinterpret_cast<CallFrame*>(top - sizeof(CallFrame));
   frame->arg = arg;
   frame->rip = 0;

   InterruptFrame* context = (InterruptFrame*)stack;

   context->gs = UserDataSegment | 3;
   context->fs = UserDataSegment | 3;
   context->es = UserDataSegment | 3;
   context->ds = UserDataSegment | 3;
   context->edi = 0x11111111;
   context->esi = 0x22222222;
   context->ebp = 0x0;
   context->edx = 0xdddddddd;
   context->ecx = 0xcccccccc;
   context->ebx = 0xbbbbbbbb;
   context->eax = 0xaaaaaaaa;
   context->interrupt = 0;
   context->error = 0;
   context->eip = static_cast<uint32_t>(main);
   context->cs = UserCodeSegment | 3;
   context->eflags = Flags::Reserved | Flags::InterruptEnable;
   context->esp = UserStackStart | 3;
   context->ss = UserDataSegment | 3;

   return stack;
}
