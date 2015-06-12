#include <SoftMutex.hh>
#include <Debug.hh>
#include <Interrupt.hh>
#include <cstdio>

//
// DO NOT CALL OTHER CLASSES/FUNCTIONS FROM HERE!
//

SoftMutex::SoftMutex()
{
   spinlock_softirq_init(&lockM);
}

SoftMutex::~SoftMutex()
{
   // assert its not held!
}

void
SoftMutex::enter()
{
   long ret = spinlock_softirq_enter(&lockM);

   KASSERT(ret == 0);
}

bool
SoftMutex::tryEnter()
{
   long ret = spinlock_softirq_exit(&lockM);

   return ret == 0;
}

void
SoftMutex::exit()
{
   long ret = spinlock_softirq_exit(&lockM);

   KASSERT(ret == 1);
}
