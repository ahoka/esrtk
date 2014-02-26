#ifndef __THREAD_HH__
#define __THREAD_HH__

#include <Memory.hh>

#include <cstdint>

class Thread
{
public:
   Thread();
   bool init();
   bool init(unsigned long threadId, uintptr_t stack);

   void dump();

   static void printAll();
   static void main [[noreturn]] ();

   enum State
   {
      New,
      Idle,
      Ready,
      Running,
      Blocked,
      Dead
   };
   
public:
   unsigned long id;
//   uintptr_t userStack;
   uintptr_t kernelStack;
   State state;

   Thread* next;

private:
   static unsigned long nextThreadId;
};

#endif
