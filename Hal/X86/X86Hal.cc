#include <Hal.hh>

#include <X86/PageDirectory.hh>
#include <X86/Processor.hh>

bool
Hal::mapPage(uintptr_t virt, uintptr_t phys)
{
   return PageDirectory::mapPage(virt, phys);
}

bool
Hal::unmapPage(uintptr_t virt)
{
   return PageDirectory::unmapPage(virt);
}

uintptr_t
Hal::getPhysicalAddress(uintptr_t virt)
{
   return PageDirectory::getPhysicalPage(virt);
}

void
Hal::disableLocalInterrupts()
{
   cli();
}

void
Hal::enableLocalInterrupts()
{
   sti();
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
      cli();
   }
   else
   {
      sti();
   }
}
