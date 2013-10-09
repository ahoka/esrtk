#ifndef RANGE_HH
#define RANGE_HH

template <class T>
class Range
{
public:
   virtual bool empty() = 0;
   virtual void popFront() = 0;
   virtual T front() = 0;
};

#endif
