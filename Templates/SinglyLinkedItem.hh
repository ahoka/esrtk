#ifndef SINGLYLINKEDITEM_HH
#define SINGLYLINKEDITEM_HH

template <class T>
class SinglyLinkedList;

template <class T>
class SinglyLinkedItem
{
public:
   SinglyLinkedItem()
   {
      setNext(static_cast<T*>(this));
   }

private:
   void insertAfter(T* item)
   {
      item->next = next;
      next = item;
   }

   T* getNext()
   {
      return next;
   }

   void setNext(T* n)
   {
      next = n;
   }

   friend class SinglyLinkedList<T>;

   T* next;
};

#endif
