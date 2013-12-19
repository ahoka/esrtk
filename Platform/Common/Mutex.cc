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
   // if (Interrupt::getInterruptLevel() > 0)
   // {
   //    return;
   // }

   long ret = spinlock_lock(&lock);
   KASSERT(ret == 0);
//   printf("Lock: %lu\n", ret);
}

bool
Mutex::tryEnter()
{
   // if (Interrupt::getInterruptLevel() > 0)
   // {
   //    return true;
   // }

//   printf("Try Lock was: %lu\n", lock);
   long ret = spinlock_try(&lock);
//   printf("Try Lock: 0x%lx\n", ret);

   return ret == 0;
}

void
Mutex::exit()
{
   // if (Interrupt::getInterruptLevel() > 0)
   // {
   //    return;
   // }

   long ret = spinlock_unlock(&lock);
//   printf("Unlock: %lu\n", ret);
   KASSERT(ret == 0);
}
