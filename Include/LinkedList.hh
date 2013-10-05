#ifndef LINKEDLIST_HH
#define LINKEDLIST_HH

template <class T>
class LinkedListItem
{
   T* prev;
   T* next;

   void insertAfter(T* item)
   {
      item->next = next;
      item->prev = this;

      next->prev = item;
      next = item;
   }
};

template <class T>
class LinkedList
{
public:
   void insertLast(T* item)
   {
      head.prev->insertAfter(item);
   }

   void insertFirst(T* item)
   {
      head.insertAfter(item);
   }

private:
   T head;
};

class Test : LinkedListItem<Test>
{
};

class TestL : LinkedList<Test>
{
};

#endif
