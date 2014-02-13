#ifndef SEGMENTLIST_HH
#define SEGMENTLIST_HH

class SegmentList
{
public:
   class Iterator
   {
   public:
      Iterator(Segment* first)
	 : next(first)
      {
      }

      bool hasNext()
      {
	 return next->getSize() != 0;
      }

      Segment& getNext()
      {
	 Segment* item = next;

	 KASSERT(item->getSize() != 0);

	 next = next->nextSegment;

	 return *next;
      }

      void remove()
      {
	 Segment* item = next->prevSegment;

	 Segment* oldNext = item->nextSegment;
	 Segment* oldPrev = item->prevSegment;
	    
	 oldPrev->nextSegment = oldNext;
	 oldNext->prevSegment = oldPrev;
      }
	 
   private:
      Segment *next;
   };

   SegmentList::Iterator getIterator()
   {
      return SegmentList::Iterator(&head);
   }

   void add(Segment* item)
   {
      Segment* prev = head.prevSegment;
      Segment* next = &head;

      item->nextSegment = next;
      item->prevSegment = prev;

      prev->nextSegment = item;
      next->prevSegment = item;
   }

private:
   Segment head;
};

#endif
