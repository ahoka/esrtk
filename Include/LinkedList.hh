#ifndef LINKEDLIST_HH
#define LINKEDLIST_HH

template <class T>
class LinkedListItem
{
   T* prev;
   T* next;

   void insertAfter(T* item)
   {
      item.next = next;
      next.prev = item;
      next = item;
      item.prev = this;
   };
};

template <class T>
class LinkedList
{
   T head;

   void insertLast(T* item)
   {
   }
};

class Test : LinkedListItem<Test>
{
};

class TestL : LinkedList<Test>
{
};

#endif
