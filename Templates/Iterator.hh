#ifndef ITERATOR_HH
#define ITERATOR_HH

template <class T>
class Iterator
{
public:
   virtual ~Iterator() = 0;

   virtual Iterator& operator ++() = 0;
   virtual T& operator *() = 0;
   virtual bool operator !=(Iterator<T>&) = 0;
};

#endif
