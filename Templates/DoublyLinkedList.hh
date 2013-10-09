#ifndef DOUBLYLINKEDLIST_HH
#define DOUBLYLINKEDLIST_HH

template <class T>
class DoublyLinkedList
{
public:
   void insertFirst(T* item)
   {
      T* oldNext = head.next;

      item->next = head.next;
      head.next = item;

      oldNext->prev = item;
      item->prev = &head;
   }

   void insertLast(T* item)
   {
      T* oldPrev = head.prev;

      item->prev = head.prev;
      head.prev = item;

      oldPrev->next = item;
      item->next = &head;
   }

   void remove(T* item)
   {
      T* oldPrev = item->prev;
      T* oldNext = item->next;

      oldPrev->next = oldNext;
      oldNext->prev = oldPrev;
   }

   T* getFirst()
   {
      T* item = 0;

      if (head.next != &head)
      {
	 item = head.next;
	 remove(head.next);
      }

      return item;
   }

   T* find(T key)
   {
      for (T* item = head.next;
	   item != &head;
	   item = item->next)
      {
	 if (item == key)
	 {
	    return item;
	 }
      }

      return 0;
   }

   void init()
   {
      head.next = &head;
      head.prev = &head;
   }

private:
   T head;
};

#endif
