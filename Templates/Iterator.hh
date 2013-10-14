#ifndef ITERATOR_HH
#define ITERATOR_HH

template <class T>
class Iterator
{
public:
   virtual T& operator ++() = 0;
   virtual T& operator *() = 0;
   bool operator !=(Iterator<T>&) = 0;
};

#endif
