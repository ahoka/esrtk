#ifndef DOUBLEENDEDRANGE_HH
#define DOUBLEENDEDRANGE_HH

#include <ForwardRange.hh>

template <class T>
class DoubleEndedRange : ForwardRange<T>
{
public:
   virtual Ref<T> back() = 0;
   virtual void popBack() = 0;
};

#endif
