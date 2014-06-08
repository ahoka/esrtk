#include <Modules.hh>
#include <Debug.hh>

#include <Multiboot.hh>
#include <Parameters.hh>

#include <cstring>

using namespace Multiboot;

extern MultibootStructure* mbd;

uintptr_t __start_modules = ~0u;
uintptr_t __end_modules = 0u;

unsigned int Modules::nextFree = 0;
Modules::Module Modules::list[ModulesMax];

const char* Modules::typeToTypename[NumberOfTypes] =
{
   "Ramdisk"
};

void
Modules::init()
{
   // this should be a MultibootModules::init() function
   if (mbd->flags & MultibootStructure::ModulesValid)
   {
      int count = 0;
      for (unsigned int i = 0; i < mbd->modulesCount; i++)
      {
	 uintptr_t moduleStart = *(uintptr_t* )mbd->modulesAddress;
	 uintptr_t moduleEnd = *(uintptr_t* )(mbd->modulesAddress + 4);
	 const char* moduleName = *(const char** )(mbd->modulesAddress + 8);
	 
	 printf("Kernel module found at: %p (%p)\n",
		(void* )moduleStart,
		(void* )(moduleStart + KernelVirtualBase));
	 
	 printf("Kernel module ends at: %p (%p)\n",
		(void *)moduleEnd,
		(void *)(moduleEnd + KernelVirtualBase));

	 printf("Module string: %s\n", moduleName);

	 unsigned long moduleSize = moduleEnd - moduleStart;
	 printf("Module size: %lu\n", moduleSize);

	 if (moduleEnd > __end_modules)
	 {
	    __end_modules = moduleEnd;
	 }

	 if (moduleStart < __start_modules)
	 {
	    __start_modules = moduleStart;
	 }

	 count++;
	 Modules::add(moduleStart + KernelVirtualBase, moduleSize, moduleName);
      }
      
      if (count)
      {
	 printf("Memory reserved for %d kernel %s: %p-%p\n",
		count, count > 1 ? "modules" : "module",
		(void*)__start_modules, (void*)__end_modules);
      }
   }
   else
   {
      printf("No kernel modules present\n");
   }
}

void
Modules::add(uintptr_t address, std::size_t size, const char* name)
{
   KASSERT(nextFree < ModulesMax);

   printf("Registering kernel module: %s (%p)\n", name, (void* )address);

   list[nextFree].address = address;
   list[nextFree].size = size;
   std::strlcpy((char *)&list[nextFree].name, name, sizeof(list[nextFree].name));

   nextFree++;
}

void
Modules::handleModules()
{
   for (unsigned int i = 0; i < nextFree; i++)
   {
      printf("Module name: %s\n", list[i].name);

      ModuleHeader* h = reinterpret_cast<ModuleHeader*>(list[i].address);

      printf("%p %p\n", h, (void*)list[i].address);

      if (h->magic != Modules:: Magic)
      {
	 printf("Module is invalid! Magic: 0x%x\n", h->magic);
      }

      if (h->type < NumberOfTypes)
      {
	 printf("Module type: %s\n", typeToTypename[h->type]);
      }
      else
      {
	 printf("Module type: 0x%x\n", h->type);
      }

      printf("Module size: %lu\n", (unsigned long )h->size);
   }
}

