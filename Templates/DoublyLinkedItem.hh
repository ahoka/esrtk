#ifndef DOUBLYLINKEDITEM_HH
#define DOUBLYLINKEDITEM_HH

template <class T>
class DoublyLinkedItem
{
public:
   DoublyLinkedItem() :
      prev(0),
      next(0)
   {
      // empty
   }

private:
   T* prev;
   T* next;
};

#endif
