#include <Kernel/Scheduler.hh>
#include <Kernel/Thread.hh>
#include <X86/ThreadContext.hh>

#include <Parameters.hh>
#include <Debug.hh>
#include <spinlock.h>

#include <list>

#include <cstdio>

using namespace Kernel;

namespace
{
   spinlock_softirq_t threadLock = SPINLOCK_SOFTIRQ_STATIC_INITIALIZER;

   std::list<Thread*>&
   getThreadList()
   {
      static std::list<Thread*> threadList;

      return threadList;
   }
};

unsigned long Thread::nextThreadId = 1;

Thread::Thread(Thread::Type type)
   : idM(-1ul),
     userStackM(0),
     kernelStackM(0),
     stateM(New),
     typeM(type),
     processM(0),
     nextM(0),
     onCpuM(0)
{
   if (type == Thread::Type::UserThread)
   {
      Debug::verbose("Creating user thread...\n");
   }
   else
   {
      Debug::verbose("Creating thread...\n");
   }
}

Thread::~Thread()
{
   // XXX free stack space!
   printf("Deleteing thread: %p...\n", this);

   Scheduler::remove(this);
}

// used when creating thread 0
bool
Thread::init0(uintptr_t stack)
{
   idM = 0;
   kernelStackM = stack;
   processM = Scheduler::getKernelProcess();

   Debug::verbose("Initializing idle thread (thread0): %p...\n", (void*)stack);

   Scheduler::insert(this);
   getThreadList().push_back(this);

   nameM = "idle";

   return true;
}

bool
Thread::init()
{
   if (typeM == Thread::Type::UserThread)
   {
      Debug::verbose("Initializing user thread %p...\n", this);
   }
   else
   {
      Debug::verbose("Initializing kernel thread %p...\n", this);
   }

   spinlock_softirq_enter(&threadLock);

   KASSERT(nextThreadId != 0);
   idM = nextThreadId++;

   bool success = Memory::createKernelStack(kernelStackM);
   if (!success)
   {
      spinlock_softirq_exit(&threadLock);
      return false;
   }

   if (typeM == UserThread)
   {
      userStackM = UserStackStart;
      kernelStackM = ThreadContext::initUserStack(kernelStackM,
                                                  CodeStart,
                                                  0xdeadbabe);
//      memcpy((void*)CodeStart, "\xb8\xfa\x00\x00\x10\x00\xe0\xff", 8);
   }
   else
   {
      kernelStackM = ThreadContext::initKernelStack(kernelStackM,
                                                    reinterpret_cast<uintptr_t>(&Thread::main),
                                                    reinterpret_cast<uintptr_t>(this));
   }

   spinlock_softirq_exit(&threadLock);

   Debug::verbose("Thread's new kernel stack is %p\n", (void*)kernelStackM);

   Scheduler::insert(this);
   getThreadList().push_back(this);

   return success;
}

bool
Thread::addJob(Job /*job*/)
{
//   jobsM.push(job);

   return true;
}

const char* threadType[] =
{
   "User",
   "Kernel",
   "Interrupt"
};
const char* threadState[] =
{
   "New",
   "Idle",
   "Ready",
   "Running",
   "Agony",
   "Dead"
};

void
Thread::dump()
{
   printf(" %lu\t%p\t%p\t%s\t%s\t%lu\n", idM, (void *)kernelStackM,
          (void*)userStackM, threadState[stateM], threadType[typeM],
          onCpuM);
}

void
Thread::printAll()
{
   printf(" id\tkstack\t\tustack\t\tstate\ttype\toncpu\n");
   for (auto& t : getThreadList())
   {
      t->dump();
   }
}

void
Thread::main(Thread* thread)
{
   Debug::verbose("Thread main called on %p (%p)!\n", thread, &thread);

   thread->stateM = Ready;

   for (;;)
   {
      if (thread->stateM == Ready)
      {
         thread->stateM = Running;
         // while (!thread->jobsM.empty())
         // {
         //    Job job = thread->jobsM.front();
         //    thread->jobsM.pop();

         //    job.execute();
         // }
         while (true) // XXX
         {
            asm volatile("pause");
         }
      }
      else if (thread->stateM == Agony)
      {
         printf("Thread %p is exiting...\n", thread);
         thread->stateM = Dead;
         for (;;)
         {
            // wait for destruction
            asm volatile("pause");
         }
      }

      thread->stateM = Idle;
   }
}

Thread*
Thread::createKernelThread(const char* name)
{
   Thread* thread = new Thread(Type::KernelThread);
   thread->processM = Scheduler::getKernelProcess();
   thread->setName(name);
   thread->init();

   printf("Kernel thread created: %s\n", name);

   return thread;
}

Thread*
Thread::createKernelThread()
{
   char threadName[32];

   Thread* thread = new Thread(Type::KernelThread);
   thread->processM = Scheduler::getKernelProcess();

   thread->init();

   // XXX should be done before init, but init gaves a thread an id
   int len = snprintf(threadName, sizeof(threadName), "KernelThread-%lu", thread->getId());
   KASSERT(len < (int)sizeof(threadName));
   thread->setName(threadName);

   printf("Kernel thread created: %s\n", threadName);

   return thread;
}

Thread*
Thread::createUserThread(Process* process)
{
   Thread* thread = new Thread(Type::UserThread);
   thread->processM = process;
   thread->init();

   return thread;
}

unsigned long
Thread::getId() const
{
   return idM;
}

void
Thread::setName(const char* name)
{
   nameM = std::string(name);
}

std::string
Thread::getName() const
{
   return nameM;
}

uintptr_t
Thread::getKernelStack() const
{
   return kernelStackM;
}

void
Thread::setKernelStack(uintptr_t stack)
{
   kernelStackM = stack;
}

uintptr_t
Thread::getUserStack() const
{
   return userStackM;
}

void
Thread::setUserStack(uintptr_t stack)
{
   userStackM = stack;
}

Thread::Type
Thread::getType() const
{
   return typeM;
}

Process*
Thread::getProcess() const
{
   return processM;
}

void
Thread::setRunning()
{
   stateM = Running;
   onCpuM++;
}

void
Thread::setReady()
{
   stateM = Ready;
}
