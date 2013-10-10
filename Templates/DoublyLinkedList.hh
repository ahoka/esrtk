#ifndef DOUBLYLINKEDLIST_HH
#define DOUBLYLINKEDLIST_HH

#include <ForwardRange.hh>

template <class T>
class DoublyLinkedList
{
public:
   DoublyLinkedList()
   {
      head.setPrev(&head);
      head.setNext(&head);
   }

   class DoublyLinkedListRange : ForwardRange<T>
   {
   public:
      DoublyLinkedListRange(DoublyLinkedList *owningList)
	 : list(owningList)
      {
	 currentItem = list->begin();
      }

      bool empty()
      {
         if (currentItem == list->end())
         {
            return true;
         }
         else
         {
            return false;
         }
      }

      void popFront()
      {
         currentItem = currentItem->next;
      }

      Ref<T> front()
      {
         return *currentItem;
      }

      ForwardRange<T> save()
      {
         return *this;
      }

   private:
      DoublyLinkedList* list;
      T* currentItem;
   };

   void insertLast(T* item)
   {
      head.getPrev()->insertAfter(item);
   }

   void insertFirst(T* item)
   {
      head.insertAfter(item);
   }

   T* begin()
   {
      return head.next;
   }

   T* end()
   {
      return &head;
   }

   DoublyLinkedListRange range()
   {
      return DoublyLinkedListRange(this);
   }

   operator DoublyLinkedListRange()
   {
      return range();
   }

private:
   T head;
};

#endif
