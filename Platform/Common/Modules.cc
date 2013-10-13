#include <Modules.hh>
#include <Debug.hh>
#include <cstring>

unsigned int Modules::nextFree = 0;
Modules::Module Modules::list[ModulesMax];

void
Modules::add(uintptr_t address, std::size_t size, const char* name)
{
   KASSERT(nextFree < ModulesMax);

   list[nextFree].address = address;
   list[nextFree].size = size;
   std::strncpy((char *)&list[nextFree].name, name, sizeof(list[nextFree].name));

   nextFree++;
}
