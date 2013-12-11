#include <Hal.hh>
#include <X86/PageDirectory.hh>

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

