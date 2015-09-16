#ifndef SYMBOLTABLE_HH
#define SYMBOLTABLE_HH

#include <map>
#include <string>
#include <utility>

class SymbolTable
{
public:
   struct Symbol
   {
      uintptr_t address;
      size_t size;
   };

   void addSymbol(std::string name, uintptr_t address, size_t size);
   std::pair<Symbol, bool> lookup(std::string name);

private:
   std::map<std::string, Symbol> symbolsM;
};

#endif
