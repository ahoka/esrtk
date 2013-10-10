#ifndef DOUBLYLINKEDITEM_HH
#define DOUBLYLINKEDITEM_HH

#include <ForwardRange.hh>

template <class T>
class DoublyLinkedList;

template <class T>
class DoublyLinkedItem
{
public:
   DoublyLinkedItem()
   {
      setPrev(static_cast<T*>(this));
      setNext(static_cast<T*>(this));
   }

   void insertAfter(T* item)
   {
      item->next = next;
      item->prev = static_cast<T*>(this);

      next->prev = item;
      next = item;
   }

private:
   T* getPrev()
   {
      return prev;
   }

   T* getNext()
   {
      return next;
   }

   void setPrev(T* p)
   {
      prev = p;
   }

   void setNext(T* n)
   {
      next = n;
   }

   friend class DoublyLinkedList<T>;

   T* prev;
   T* next;
};

#endif
