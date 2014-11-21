#ifndef SOFTMUTEX_HH
#define SOFTMUTEX_HH

#include <Mutex.hh>
#include <spinlock.h>

class SpinMutex : public Mutex
{
public:
   SpinMutex();
   virtual ~SpinMutex();

   virtual void enter();
   virtual bool tryEnter();
   virtual void exit();

private:
   spinlock_t lockM;
};

#endif
