#ifndef TEMPLATES_HH
#define TEMPLATES_HH

#include <cstddef>

//
// Various helper templates
//

template <uint64_t>
uint64_t roundTo(uint64_t num, uint32_t size)
{
   uint64_t orig = reinterpret_cast<uint64_t>(num);
   uint64_t rounded = (orig + (size - 1)) & ~size;
   
   return reinterpret_cast<uint64_t>(rounded);
}

template <class T>
T roundTo(T num, uint32_t size)
{
   uint32_t orig = reinterpret_cast<size_t>(num);
   uint32_t rounded = (orig + (size - 1)) & ~size;
   
   return reinterpret_cast<T>(rounded);
}

#endif
