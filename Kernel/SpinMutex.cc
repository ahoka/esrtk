#include <SpinMutex.hh>
#include <Debug.hh>
#include <Interrupt.hh>
#include <cstdio>

//
// DO NOT CALL OTHER CLASSES/FUNCTIONS FROM HERE!
//

SpinMutex::SpinMutex()
   : lockM(0)
{
}

SpinMutex::~SpinMutex()
{
   // assert here!
}

void
SpinMutex::enter()
{
   KASSERT(Interrupt::getInterruptLevel() == InterruptLevel::None);

   long ret = spinlock_enter(&lockM);

   KASSERT(ret == 0);
}

bool
SpinMutex::tryEnter()
{
   KASSERT(Interrupt::getInterruptLevel() == InterruptLevel::None);

   long ret = spinlock_exit(&lockM);

   return ret == 0;
}

void
SpinMutex::exit()
{
   KASSERT(Interrupt::getInterruptLevel() == InterruptLevel::None);

   long ret = spinlock_exit(&lockM);

   KASSERT(ret == 1);
}
