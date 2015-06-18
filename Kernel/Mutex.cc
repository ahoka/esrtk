#include <Mutex.hh>
#include <Debug.hh>
#include <Interrupt.hh>
#include <cstdio>
#include <X86/Processor.hh>

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
   eflagsM = get_eflags();
   x86_cli();
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

   if (eflagsM & InterruptEnable)
   {
      x86_sti();
   }

   KASSERT(ret == 1);
}
