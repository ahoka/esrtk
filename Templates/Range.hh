#ifndef RANGE_HH
#define RANGE_HH

#include <Ref.hh>

template <class T>
class Range
{
public:
   Range()
   {
   }

   virtual ~Range()
   {
   }

   virtual bool empty() = 0;
   virtual void popFront() = 0;
   virtual Ref<T> front() = 0;
};

#endif
