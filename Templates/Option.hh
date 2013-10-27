#ifndef __OPTION_HH__
#define __OPTION_HH__

#include <None.hh>

// Option class, a bit like in Scala
//

template <class T>
class Option
{
public:
   bool isEmpty()
   {
      return empty;
   }

   bool isDefined()
   {
      return !empty;
   }

   T get()
   {
      return some;
   }

   Option<T> orElse(T alternative)
   {
      if (isEmpty)
      {
	 return alternative;
      }
      else
      {
	 return *this;
      }
   }

   T getOrElse(T alternative)
   {
      if (isEmpty)
      {
	 return alternative;
      }
      else
      {
	 return some;
      }
   }

private:
   bool empty;
   T some;
};

#endif
