#ifndef __THREAD_HH__
#define __THREAD_HH__

#include <cstdint>

class Thread
{
public:
   unsigned long id;
//   uintptr_t userStack;
   uintptr_t kernelStack;

   Thread* getCurrentThread();

   Thread(unsigned long threadId, uintptr_t KernelStack)
      : id(threadId),
	kernelStack(KernelStack)
   {
   }
};

#endif
