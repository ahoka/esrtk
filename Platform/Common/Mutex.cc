#include <Mutex.hh>
#include <Debug.hh>
#include <cstdio>

Mutex::Mutex()
   : lock(0)
{
   printf("Mutex()\n");
}

void
Mutex::enter()
{
   long ret = spinlock_lock(&lock);
   KASSERT(ret == 0);
//   printf("Lock: %lu\n", ret);
}

bool
Mutex::tryEnter()
{
//   printf("Try Lock was: %lu\n", lock);
   long ret = spinlock_try(&lock);
//   printf("Try Lock: 0x%lx\n", ret);

   return ret == 0;
}

void
Mutex::exit()
{
   long ret = spinlock_unlock(&lock);
//   printf("Unlock: %lu\n", ret);
   KASSERT(ret == 0);
}

