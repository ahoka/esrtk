#include <Modules.hh>
#include <Debug.hh>

#include <Multiboot.hh>
#include <Parameters.hh>

#include <cstring>

extern Multiboot* mbd;

uintptr_t __start_modules = ~0u;
uintptr_t __end_modules = 0u;

unsigned int Modules::nextFree = 0;
Modules::Module Modules::list[ModulesMax];

void
Modules::init()
{
   // this should be a MultibootModules::init() function
   if (mbd->flags & Multiboot::ModulesValid)
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
		(void *)moduleName,
		(void *)(moduleName + KernelVirtualBase));

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
