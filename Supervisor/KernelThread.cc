#include <KernelThread.hh>

#include <X86/Assembly.hh>

KernelThread::KernelThread() :
   stackPointer(0),
   basePointer(0),
   instructionPointer(0)
{
}

void
KernelThread::create(uintptr_t stack, uintptr_t function)
{
   stackPointer = stack;
   instructionPointer = function;
}

void
KernelThread::yield()
{
   asm volatile("movl %%esp, %0" : "=r"(stackPointer));
   asm volatile("movl %%ebp, %0" : "=r"(basePointer));

   instructionPointer = get_eip();

   // XXX scheduler call, possibly add a magic value to eax
}
