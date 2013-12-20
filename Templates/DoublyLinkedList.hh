#ifndef DOUBLYLINKEDLIST_HH
#define DOUBLYLINKEDLIST_HH

#include <DoubleEndedRange.hh>
#include <Iterator.hh>

#include <cstddef>

template <class T>
class DoublyLinkedList
{
public:
   DoublyLinkedList()
      : count(0),
	head()
   {
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

   class DoublyLinkedListIterator
   {
   private:
      DoublyLinkedListIterator(DoublyLinkedList* owningList)
	 : list(owningList),
           currentItem(list->firstItem())
      {
      }

      DoublyLinkedListIterator(DoublyLinkedList* owningList, T* pointAt)
	 : list(owningList),
           currentItem(pointAt)
      {
      }

   public:
      DoublyLinkedListIterator& operator ++()
      {
         currentItem = currentItem->next;

         return *this;
      }

      T& operator *()
      {
         return *currentItem;
      }

      bool operator !=(DoublyLinkedListIterator& other)
      {
	 return currentItem != other.currentItem;
      }

   private:
      DoublyLinkedList* list;
      T* currentItem;

      friend class DoublyLinkedList<T>;
   };

   void insertLast(T* item)
   {
      head.getPrev()->insertAfter(item);
      count++;
   }

   void insertFirst(T* item)
   {
      head.insertAfter(item);
      count++;
   }

   void remove(T* item)
   {
      item->unlink();
      count--;
   }

   DoublyLinkedListRange range()
   {
      return DoublyLinkedListRange(this);
   }

   operator DoublyLinkedListRange()
   {
      return range();
   }

   DoublyLinkedListIterator begin()
   {
      return DoublyLinkedListIterator(this);
   }

   DoublyLinkedListIterator end()
   {
      return DoublyLinkedListIterator(this, lastItem());
   }

   std::size_t getCount()
   {
      return count;
   }

private:
   T* firstItem()
   {
      return head.next;
   }

   T* lastItem()
   {
      return &head;
   }

   std::size_t count;
   T head;
};

#endif
