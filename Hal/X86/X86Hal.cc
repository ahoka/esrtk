#include <Hal.hh>

#include <X86/PageDirectory.hh>
#include <X86/Processor.hh>

void
Hal::disableLocalInterrupts()
{
   x86_cli();
}

void
Hal::enableLocalInterrupts()
{
   x86_sti();
}

void
Hal::saveLocalInterrupts(InterruptFlags& flags)
{
   if (get_eflags() & InterruptEnable)
   {
      flags = 1;
   }
   else
   {
      flags = 0;
   }
}

void
Hal::restoreLocalInterrupts(InterruptFlags& flags)
{
   if (flags == 0)
   {
      x86_cli();
   }
   else
   {
      x86_sti();
   }
}
