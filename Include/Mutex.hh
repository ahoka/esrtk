#ifndef MUTEX_HH
#define MUTEX_HH

#include <spinlock.h>
#include <cstdint>

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
   uint32_t eflagsM;
   spinlock_t lockM;
};

#endif
