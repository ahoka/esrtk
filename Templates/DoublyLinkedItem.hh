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
      : prev(static_cast<T*>(this)),
	next(static_cast<T*>(this))
   {
   }

   virtual ~DoublyLinkedItem() = default;

   DoublyLinkedItem(const DoublyLinkedItem&)
   {
   }

   DoublyLinkedItem& operator = (const DoublyLinkedItem&)
   {
   }

private:
   void insertAfter(T* item)
   {
      item->next = next;
      item->prev = static_cast<T*>(this);

      next->prev = item;
      next = item;
   }

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

   void unlink()
   {
      prev->next = next;
      next->prev = prev;

      prev = next = 0; // XXX
   }

   friend class DoublyLinkedList<T>;

   T* prev;
   T* next;
};

#endif
