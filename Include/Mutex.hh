#ifndef __MUTEX_HH__
#define __MUTEX_HH__

#include <spinlock.h>

class Mutex
{
public:
   Mutex();

   void enter();
   bool tryEnter();
   void exit();

private:
   spinlock_t lock;
};

#endif
