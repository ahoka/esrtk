#include <Hal.hh>
#include <Thread.hh>
#include <Parameters.hh>

#include <X86/ThreadContext.hh>

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
   context->edi = 0xd;
   context->esi = 0x5;
   context->ebp = 0x0;
   context->esp = top + 8; // top of the stack
   context->edx = 0xf;
   context->ecx = 0xc;
   context->ebx = 0xb;
   context->eax = 0xa;
   context->interrupt = 0;
   context->error = 0;
   context->rip = reinterpret_cast<uint32_t>(&Thread::main); // XXX make this configurable
   context->cs = KernelCodeSegment;
   context->eflags = 0x2;
   context->useresp = 0x12345678;
   context->ss = KernelDataSegment;

   printf("----\n%p\n", context);
   printf("----\n");
   context->print();
   printf("----\n");

   // const std::size_t initSize = (5 * 4) + (12 * 4);

   // uint32_t* sp = reinterpret_cast<uint32_t*>(top - initSize);

   // sp[12] = reinterpret_cast<uint32_t>(&Thread::main);
   // sp[13] = KernelCodeSegment;
   // sp[14] = 0x2; // XXX magic number
   // sp[15] = 0; // these should be discarded
   // sp[16] = 0; //

   // sp[11] = 0x0; //
   // sp[10] = 0x0; //
   // sp[9] = 0x0; //
   // sp[8] = 0x0; //
   // sp[7] = 0x0; //
   // sp[6] = 0x0; //
   // sp[5] = 0x0; //
   // sp[4] = 0x0; //
   // sp[3] = KernelDataSegment; //
   // sp[2] = KernelDataSegment; //
   // sp[1] = KernelDataSegment; //
   // sp[0] = KernelDataSegment; //

   // // subtract the above and the registers
   // return top - initSize;

   return stack;
}
