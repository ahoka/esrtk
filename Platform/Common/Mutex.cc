#include <Mutex.hh>
#include <Debug.hh>
#include <Interrupt.hh>
#include <cstdio>

//
// DO NOT CALL OTHER CLASSES/FUNCTIONS FROM HERE!
//

Mutex::Mutex()
   : lock(0)
{
}

void
Mutex::enter()
{
   KASSERT(Interrupt::getInterruptLevel() == InterruptLevel::None);
   // if (Interrupt::getInterruptLevel() > 0)
   // {
   //    return;
   // }

   long ret = spinlock_enter(&lock);
   KASSERT(ret == 0);
//   printf("Lock: %lu\n", ret);
}

bool
Mutex::tryEnter()
{
   KASSERT(Interrupt::getInterruptLevel() == InterruptLevel::None);
   // if (Interrupt::getInterruptLevel() > 0)
   // {
   //    return true;
   // }

//   printf("Try Lock was: %lu\n", lock);
   long ret = spinlock_exit(&lock);
//   printf("Try Lock: 0x%lx\n", ret);

   return ret == 0;
}

void
Mutex::exit()
{
   KASSERT(Interrupt::getInterruptLevel() == InterruptLevel::None);
   // if (Interrupt::getInterruptLevel() > 0)
   // {
   //    return;
   // }

   long ret = spinlock_exit(&lock);
//   printf("Unlock: %lu\n", ret);
   KASSERT(ret == 1);
}
