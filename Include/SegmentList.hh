#ifndef SEGMENTLIST_HH
#define SEGMENTLIST_HH

#include <Segment.hh>

class SegmentList
{
public:
   SegmentList()
      : headM(),
        countM(0)
   {
   }

   class Iterator
   {
   public:
      Iterator()
         : next(nullptr)
      {
      }

      Iterator(const Iterator& other)
         : next(other.next)
      {
      }

      Iterator& operator=(const Iterator& other)
      {
         if (&other != this)
         {
            next = other.next;
         }

         return *this;         
      }

      bool operator==(const Iterator& other)
      {
         return next == other.next;
      }

      bool operator!=(const Iterator& other)
      {
         return !(*this == other);
      }

      Segment* operator*()
      {
	 return next;
      }

      Segment* operator->()
      {
         return next;
      }

      Iterator& operator++()
      {
	 next = next->nextM;

         return *this;
      }

      Iterator operator++(int)
      {
         Segment* oldNext = next;
         next = next->nextM;

         return Iterator(oldNext);
      }

   private:
      Iterator(Segment* next)
	 : next(next)
      {
      }

      Segment *next;

      friend class SegmentList;
   };

   SegmentList::Iterator begin()
   {
      return SegmentList::Iterator(this->headM.nextM);
   }

   SegmentList::Iterator end()
   {
      SegmentList::Iterator iterator;
      iterator.next = &headM;

      return iterator;
   }

   void add(Segment* item)
   {
      Segment* prev = headM.prevM;
      Segment* next = &headM;

      item->nextM = next;
      item->prevM = prev;
      prev->nextM = item;
      next->prevM = item;

      ++countM;
   }

   Iterator remove(Iterator iterator)
   {
      Segment* item = iterator.next;

      Segment* oldNext = item->nextM;
      Segment* oldPrev = item->prevM;

      oldPrev->nextM = oldNext;
      oldNext->prevM = oldPrev;

      --countM;

      return Iterator(oldNext);
   }

   unsigned long count()
   {
      return countM;
   }

private:
   Segment headM;
   unsigned long countM;
};

#endif
