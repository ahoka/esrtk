#ifndef DOUBLYLINKEDLIST_HH
#define DOUBLYLINKEDLIST_HH

#include <DoubleEndedRange.hh>

template <class T>
class DoublyLinkedList
{
public:
   DoublyLinkedList()
      // head.prev(head),
      // head.next(head)
   {
      // head.setPrev(&head);
      // head.setNext(&head);
   }

   class DoublyLinkedListRange : public DoubleEndedRange<T>
   {
   public:
      DoublyLinkedListRange(DoublyLinkedList* owningList)
	 : list(owningList),
           firstItem(list->begin()),
           lastItem(list->end())
      {
      }

      bool empty()
      {
         if (firstItem == lastItem)
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
         firstItem = firstItem->next;
      }

      Ref<T> front()
      {
         return *firstItem;
      }

      void popBack()
      {
         lastItem = lastItem->prev;
      }

      Ref<T> back()
      {
         return *lastItem->prev;
      }

      ForwardRange<T> save()
      {
         return *this;
      }

   private:
      DoublyLinkedList* list;
      T* firstItem;
      T* lastItem;
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
