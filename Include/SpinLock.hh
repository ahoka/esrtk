#ifndef __SPINLOCK_HH__
#define __SPINLOCK_HH__

// does not belong here
//
extern "C" void spinlock_lock(long* lock);
extern "C" void spinlock_unlock(long* lock);
// --

class SpinLock
{
public:
   SpinLock();

   void enter();
   void exit();

private:
   long lock;
};

#endif
