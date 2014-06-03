#include <spinlock.h>
#include <X86/Processor.hh>

extern "C" void
spinlock_softirq_init(spinlock_softirq_t* lock)
{
   lock->lock = SPINLOCK_STATIC_INITIALIZER;
   lock->flags = 0;
}

extern "C" long
spinlock_softirq_enter(spinlock_softirq_t* lock)
{
   lock->flags = get_eflags();
   x86_cli();

   return spinlock_enter(&lock->lock);
}

extern "C" long
spinlock_softirq_exit(spinlock_softirq_t* lock)
{
   if (lock->flags & InterruptEnable)
   {
      x86_sti();
   }

   return spinlock_exit(&lock->lock);
}
