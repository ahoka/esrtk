#include <Kernel/SymbolTable.hh>

void
SymbolTable::addSymbol(std::string name, uintptr_t address, size_t size)
{
   symbolsM.insert({name, {address, size}});
}

std::pair<SymbolTable::Symbol, bool>
SymbolTable::lookup(std::string name)
{
   auto item = symbolsM.find(name);
   if (item == symbolsM.end())
   {
      return {{0, 0}, false};
   }
   else
   {
      return {item->second, true};
   }
}
