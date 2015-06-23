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
   }
}
