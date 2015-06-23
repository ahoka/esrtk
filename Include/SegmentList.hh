#ifndef SEGMENTLIST_HH
#define SEGMENTLIST_HH

#include <Segment.hh>

class SegmentList
{
public:
   SegmentList()
      : countM(0)
   {
   }

   class Iterator
   {
   public:
      Iterator(SegmentList* list)
	 : next(list->head.nextSegment),
           segmentList(list)
      {
      }

      bool hasNext()
      {
	 return next != &segmentList->head;
      }

      Segment& getNext()
      {
	 Segment* item = next;

	 KASSERT(item != &segmentList->head);

	 next = next->nextSegment;

	 return *item;
      }

      void remove()
      {
         KASSERT(next != &segmentList->head);

	 Segment* item = next->prevSegment;

	 Segment* oldNext = item->nextSegment;
	 Segment* oldPrev = item->prevSegment;

	 oldPrev->nextSegment = oldNext;
	 oldNext->prevSegment = oldPrev;

         --segmentList->countM;
      }

   private:
      Segment *next;
      SegmentList *segmentList;
   };

   SegmentList::Iterator getIterator()
   {
      return SegmentList::Iterator(this);
   }

   void add(Segment* item)
   {
      Segment* prev = head.prevSegment;
      Segment* next = &head;

      item->nextSegment = next;
      item->prevSegment = prev;

      prev->nextSegment = item;
      next->prevSegment = item;

      ++countM;
   }

   unsigned long count()
   {
      return countM;
   }

private:
   Segment head;
   unsigned long countM;
};

#endif
