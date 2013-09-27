#ifndef KERNELTHREAD_HH
#define KERNELTHREAD_HH

#include <cstdint>

class KernelThread
{
public:
   KernelThread();
   ~KernelThread();

   void create(uintptr_t stack, uintptr_t function);
   void destroy();
   void yield();

private:
   uint32_t id;
   uintptr_t stackPointer;
   uintptr_t basePointer;
   uintptr_t instructionPointer;

   KernelThread* prev;
   KernelThread* next;
};

#endif

