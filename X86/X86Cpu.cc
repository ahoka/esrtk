#include <Kernel/Cpu.hh>

#include <X86/PageDirectory.hh>
#include <X86/Processor.hh>

void
Cpu::disableLocalInterrupts()
{
   x86_cli();
}

void
Cpu::enableLocalInterrupts()
{
   x86_sti();
}

void
Cpu::saveLocalInterrupts(InterruptFlags& flags)
{
   if (x86_get_eflags() & InterruptEnable)
   {
      flags = 1;
   }
   else
   {
      flags = 0;
   }
}

void
Cpu::restoreLocalInterrupts(InterruptFlags& flags)
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

uint64_t
Cpu::getTimestamp()
{
   return x86_rdtsc();
}
