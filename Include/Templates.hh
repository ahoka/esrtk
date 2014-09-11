#ifndef TEMPLATES_HH
#define TEMPLATES_HH

#include <cstddef>
#include <cstdint>

//
// Various helper templates
//

template <uint64_t>
uint64_t roundTo(uint64_t num, uint32_t size)
{
   uint64_t orig = static_cast<uint64_t>(num);
   uint64_t rounded = (orig + (size - 1)) & ~(size - 1);
   
   return static_cast<uint64_t>(rounded);
}

template <class T>
T roundTo(T num, uint32_t size)
{
   uint32_t orig = uint32_t(num);
   uint32_t rounded = (orig + (size - 1)) & ~(size - 1);
   
   return T(rounded);
}

template <uint64_t>
uint64_t roundDown(uint64_t num, uint32_t size)
{
   uint64_t orig = static_cast<uint64_t>(num);
   uint64_t rounded = orig & ~(size - 1);

   return static_cast<uint64_t>(rounded);
}

template <class T>
T roundDown(T num, uint32_t size)
{
   uint32_t orig = static_cast<uint32_t>(num);
   uint32_t rounded = orig & ~(size - 1);
   
   return static_cast<T>(rounded);
}

template <class T>
T max(T first, T second)
{
   if (first > second)
   {
      return first;
   }
   else
   {
      return second;
   }
}

#endif
