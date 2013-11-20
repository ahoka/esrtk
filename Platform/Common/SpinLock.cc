#include <SpinLock.hh>

SpinLock::SpinLock()
   : lock(0)
{
}

void
SpinLock::enter()
{
   spinlock_lock(&lock);
}

void
SpinLock::exit()
{
   spinlock_unlock(&lock);
}

