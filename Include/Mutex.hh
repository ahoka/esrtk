#ifndef MUTEX_HH
#define MUTEX_HH

#include <spinlock.h>

class Mutex
{
public:
   Mutex(int flags = 0);
   ~Mutex();

   void enter();
   bool tryEnter();
   void exit();

private:
   int flagsM;
   unsigned long eflagsM;
   spinlock_t lockM;
};

#endif
