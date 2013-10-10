#ifndef RANDOMACCESSRANGE_HH
#define RANDOMACCESSRANGE_HH

#include <ForwardRange.hh>
#include <cstdint>

template <class T>
class RandomAccessRange : ForwardRange<T>
{
public:
   virtual Ref<T> at(std::size_t position);
   virtual RandomAccessRange<T> slice(std::size_t begin, std::size_t end);
};

#endif
