#ifndef SINGLYLINKEDLIST_HH
#define SINGLYLINKEDLIST_HH

#include <ForwardRange.hh>

template <class T>
class SinglyLinkedList
{
public:
   SinglyLinkedList()
   {
      // empty
   }

   class SinglyLinkedListRange : public ForwardRange<T>
   {
   public:
      SinglyLinkedListRange(SinglyLinkedList* owningList)
	 : list(owningList),
           firstItem(list->firstItem())
      {
      }

      bool empty()
      {
         if (firstItem == list->lastItem())
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

//      ForwardRange<T> save()
//      {
//         return *this;
//      }

   private:
      SinglyLinkedList* list;
      T* firstItem;
   };

   void insertLast(T* item)
   {
      T* last = firstItem();

      if (lastItem() != firstItem())
      {
	 while (last->next == lastItem())
	 {
	    last = last->next;
	 }
      }

      last->insertAfter(item);
   }
      
   void insertFirst(T* item)
   {
      head.insertAfter(item);
   }

   T* firstItem()
   {
      return head.next;
   }

   T* lastItem()
   {
      return &head;
   }

   SinglyLinkedListRange range()
   {
      return SinglyLinkedListRange(this);
   }

   operator SinglyLinkedListRange()
   {
      return range();
   }

private:
   T head;
};

#endif
