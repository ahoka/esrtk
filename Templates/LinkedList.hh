#ifndef LINKEDLIST_HH
#define LINKEDLIST_HH

template <class T>
class LinkedList;

template <class T>
class LinkedListItem
{
public:
   LinkedListItem() :
      prev(0),
      next(0)
   {
      // empty
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

   class Iterator
   {
   public:
      Iterator(LinkedList *owningList)
	 : list(owningList)
      {
	 reset();
      }

      void reset()
      {
	 currentItem = list->begin();
      }

      Iterator& operator++()
      {
	 currentItem = currentItem->next;

	 return *this;
      }

      T& operator*()
      {
	 return *currentItem;
      }

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

   Iterator getIterator()
   {
      return Iterator(this);
   }

   T* end()
   {
      return &head;
   }

private:
   T head;
};

#endif
