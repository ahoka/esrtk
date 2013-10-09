#ifndef FORWARDRANGE_HH
#define FORWARDRANGE_HH

#include <Range.hh>

template <class T>
class ForwardRange : Range<T>
{
public:
   virtual ForwardRange<T> save() = 0;
};

#endif
