#ifndef SOFTMUTEX_HH
#define SOFTMUTEX_HH

#include <Mutex.hh>
#include <spinlock.h>

class SoftMutex : public Mutex
{
public:
   SoftMutex();
   virtual ~SoftMutex();

   virtual void enter();
   virtual bool tryEnter();
   virtual void exit();

private:
   spinlock_softirq_t lockM;
};

#endif
