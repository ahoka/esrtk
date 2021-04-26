#include <Kernel/Scheduler.hh>
#include <Kernel/Thread.hh>
#include <Kernel/Power.hh>
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

   volatile uint64_t nextId = 2;

   uint64_t getNextId()
   {
      return __sync_fetch_and_add(&nextId, 1);
   }
};

Thread::Thread(Thread::Type type)
   : idM(getNextId()),
     userStackM(0),
     kernelStackM(0),
     stateM(Blocked),
     typeM(type),
     processM(0),
     nextM(0),
     onCpuM(0)
{
}

Thread::~Thread()
{
   // XXX free stack space!
   printf("Deleting thread: %p...\n", this);

   spinlock_softirq_enter(&threadLock);
   getThreadList().remove(this);
   spinlock_softirq_exit(&threadLock);
   
   Scheduler::remove(this);
}

// used when creating thread 0
bool
Thread::init0(uintptr_t stack)
{
   idM = 0;
   stateM = Running;
   nameM = "Kernel";

   kernelStackM = stack;
   processM = Scheduler::getKernelProcess();

   Debug::verbose("Initializing thread0: %p...\n", (void*)stack);

   spinlock_softirq_enter(&threadLock);
   getThreadList().push_back(this);
   spinlock_softirq_exit(&threadLock);

   return true;
}

bool
Thread::initIdle()
{
   Debug::verbose("Initializing idle thread...");

   idM = 1;
   stateM = Ready;
   nameM = "Idle";

   bool success = Memory::createKernelStack(kernelStackM);
   if (!success)
   {
      return false;
   }
   
   processM = Scheduler::getKernelProcess();

   kernelStackM = ThreadContext::initKernelStack(kernelStackM,
                                                reinterpret_cast<uintptr_t>(&Power::onIdle), 0);

   spinlock_softirq_enter(&threadLock);
   getThreadList().push_back(this);
   spinlock_softirq_exit(&threadLock);

   return true;
}

bool
Thread::init()
{
   if (typeM == Thread::Type::UserThread)
   {
      Debug::verbose("Initializing user thread %p, TID: %llu...\n", this, idM);
   }
   else
   {
      Debug::verbose("Initializing kernel thread %p, TID: %llu...\n", this, idM);
   }

   spinlock_softirq_enter(&threadLock);

   KASSERT(idM > 2);

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
   spinlock_softirq_enter(&threadLock);
   getThreadList().push_back(this);
   spinlock_softirq_exit(&threadLock);

   return success;
}

bool
Thread::addJob(const Job& job)
{
   Debug::verbose("Adding job to Thread %llu\n", this->idM);

   lockM.enter();
   jobsM.emplace(job);
   lockM.exit();

   Scheduler::setReady(this);

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
   "Ready",
   "Blocked",
   "Running"
};

void
Thread::dump()
{
   printf(" %llu\t%p\t%p\t%s\t%s\t%lu\t%s\n", idM, (void *)kernelStackM,
          (void*)userStackM, threadState[stateM], threadType[typeM],
          onCpuM, nameM.c_str());
}

void
Thread::printAll()
{
   printf(" id\tkstack\t\tustack\t\t\tstate\ttype\toncpu\tname\n");
   spinlock_softirq_enter(&threadLock);
   for (auto& t : getThreadList())
   {
      t->dump();
   }
   spinlock_softirq_exit(&threadLock);
}

void
Thread::main(Thread* thread)
{
   printf("Thread main called on %p (%p)!\n", thread, &thread);

   for (;;)
   {
      thread->lockM.enter();
      while (!thread->jobsM.empty())
      {
         printf("Running job\n");
         Job job = thread->jobsM.back();
         thread->jobsM.pop();

         thread->lockM.exit();
         job.execute();
         thread->lockM.enter();
      }
      thread->stateM = Blocked;
      thread->lockM.exit();
   }
}

Thread*
Thread::createKernelThread(const char* name)
{
   printf("Creating kernel thread: %s\n", name);

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
   int len = snprintf(threadName, sizeof(threadName), "KernelThread-%llu", thread->getId());
   KASSERT(len < (int)sizeof(threadName));
   thread->setName(threadName);
   thread->init();

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

uint64_t
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

void
Thread::setBlocked()
{
   stateM = Blocked;
}

bool
Thread::isReady()
{
   return stateM == Ready;
}

bool
Thread::isRunning()
{
   return stateM == Running;
}

bool
Thread::isBlocked()
{
   return stateM == Blocked;
}
