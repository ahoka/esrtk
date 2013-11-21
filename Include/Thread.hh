#ifndef __THREAD_HH__
#define __THREAD_HH__

#include <Memory.hh>

#include <DoublyLinkedItem.hh>
#include <DoublyLinkedList.hh>

#include <cstdint>

class Thread : public DoublyLinkedItem<Thread>
{
public:
   Thread();
   bool init();
   bool init(unsigned long threadId, uintptr_t stack);

//   Thread* getCurrentThread();
   static void printAll();
   
public:
   unsigned long id;
//   uintptr_t userStack;
   uintptr_t kernelStack;

private:
   static unsigned long nextThreadId;
   static DoublyLinkedList<Thread> list;
};

#endif
