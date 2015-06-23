#include <SegmentList.hh>

#include "catch.hpp"

SCENARIO("Kernel heap allocator SegmentList", "[kernelheap]")
{
   GIVEN("A segment list")
   {
      SegmentList list;
      Segment* s2 = new Segment();
      Segment* s3 = new Segment();
      Segment* s1 = new Segment();

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
         list.add(s1);

         AND_WHEN("An iterator is created")
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

         AND_WHEN("An iterator is created")
         {
            auto i = list.getIterator();

            THEN("The item count is one")
            {
               REQUIRE(list.count() == 1);

               AND_WHEN("Calling remove on the iterator")
               {
                  i.remove();

                  THEN("There is no next item")
                  {
                     REQUIRE(i.hasNext() == false);

                     AND_THEN("The count is zero")
                     {
                        REQUIRE(list.count() == 0);
                     }
                  }
               }
            }
         }
      }

      WHEN("Three items are added to the list")
      {
         list.add(s1);
         list.add(s2);
         list.add(s3);

         AND_WHEN("An iterator is created")
         {
            auto i = list.getIterator();

            THEN("The the items can be accessed in order")
            {
               REQUIRE(i.hasNext());
               CHECK(&i.getNext() == s1);
            }
         }
      }
   }
}
