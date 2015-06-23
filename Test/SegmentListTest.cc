#include <SegmentList.hh>

#include "catch.hpp"

SCENARIO("Kernel heap allocator SegmentList", "[kernelheap]")
{
   GIVEN("A segment list")
   {
      SegmentList list;

      WHEN("The list is empty")
      {
         THEN("The count of the list is zero")
         {
            CHECK(list.count() == 0);
         }

         AND_WHEN("Creating an iterator")
         {
            auto i = list.getIterator();

            THEN("The iterator has no next item")
            {
               CHECK(i.hasNext() == false);
            }
         }
      }

      WHEN("An item is added")
      {
         Segment* s1 = new Segment();
         s1->setSize(1);

         list.add(s1);

         AND_WHEN("Creating an iterator")
         {
            auto i = list.getIterator();

            THEN("Iterator has a next item")
            {
               CHECK(i.hasNext() == true);

               AND_THEN("The next item is the item added")
               {
                  CHECK(&i.getNext() == s1);

                  AND_THEN("The iterator has no next item")
                  {
                     CHECK(i.hasNext() == false);
                  }
               }
            }
         }
      }

      WHEN("Three items are present")
      {
         Segment* s2 = new Segment();
         Segment* s3 = new Segment();

         s2->setSize(2);
         s3->setSize(3);

         list.add(s2);
         list.add(s3);
      }
   }
}
