#ifndef LINKEDLIST_HH
#define LINKEDLIST_HH

#include <ForwardRange.hh>

template <class T>
class LinkedList;

template <class T>
class LinkedListItem
{
public:
   LinkedListItem()
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

   friend class LinkedList<T>;

   T* prev;
   T* next;
};

template <class T>
class LinkedList
{
public:
   LinkedList()
   {
      head.setPrev(&head);
      head.setNext(&head);
   }

   class LinkedListRange : ForwardRange<T>
   {
   public:
      LinkedListRange(LinkedList *owningList)
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
      LinkedList* list;
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

   LinkedListRange range()
   {
      return LinkedListRange(this);
   }

   operator LinkedListRange()
   {
      return range();
   }

private:
   T head;
};

#endif
