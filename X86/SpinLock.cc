#include <spinlock.h>
#include <Kernel/Cpu.hh>

extern "C" bool
spinlock_is_held(spinlock_t* lock)
{
   return *lock != SPINLOCK_STATIC_INITIALIZER;
}

extern "C" void
spinlock_softirq_init(spinlock_softirq_t* lock)
{
   lock->lock = SPINLOCK_STATIC_INITIALIZER;
   lock->flags = 0;
}

extern "C" long
spinlock_softirq_enter(spinlock_softirq_t* lock)
{
   Cpu::saveLocalInterrupts(lock->flags);
   Cpu::disableLocalInterrupts();

   return spinlock_enter(&lock->lock);
}

extern "C" long
spinlock_softirq_exit(spinlock_softirq_t* lock)
{
   long ret = spinlock_exit(&lock->lock);

   Cpu::restoreLocalInterrupts(lock->flags);

   return ret;
}

extern "C" bool
spinlock_softirq_is_held(spinlock_softirq_t* lock)
{
  return spinlock_is_held(&lock->lock);
}
