#include <Mutex.hh>
#include <Debug.hh>
#include <cstdio>
#include <Kernel/Cpu.hh>

//
// DO NOT CALL OTHER CLASSES/FUNCTIONS FROM HERE!
//

Mutex::Mutex(int flags)
   : flagsM(flags),
     eflagsM(0),
     lockM(SPINLOCK_STATIC_INITIALIZER)
{
}

Mutex::~Mutex()
{
   // assert its not held!
}

void
Mutex::enter()
{
   Cpu::saveLocalInterrupts(eflagsM);
   Cpu::disableLocalInterrupts();
   long ret = spinlock_enter(&lockM);

   KASSERT(ret == 0);
}

bool
Mutex::tryEnter()
{
   long ret = spinlock_exit(&lockM);

   return ret == 0;
}

void
Mutex::exit()
{
   long ret = spinlock_exit(&lockM);

   Cpu::restoreLocalInterrupts(eflagsM);

   KASSERT(ret == 1);
}
