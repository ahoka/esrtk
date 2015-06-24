#include <SegmentList.hh>

#include "catch.hpp"

SCENARIO("Kernel heap allocator SegmentList", "[kernelheap]")
{
   GIVEN("A segment list")
   {
      SegmentList list;
      Segment s2;
      Segment s3;
      Segment s1;

      WHEN("The list is empty")
      {
         THEN("The count of the list is zero")
         {
            CHECK(list.count() == 0);
         }

         AND_WHEN("Creating an iterator")
         {
            auto i = list.begin();

            THEN("The iterator has no next item")
            {
               CHECK(i == list.end());
            }
         }
      }

      WHEN("An item is added")
      {
         list.add(&s1);

         AND_WHEN("An iterator is created")
         {
            auto i = list.begin();

            THEN("Iterator has a next item")
            {
               CHECK(i != list.end());

               AND_THEN("The next item is the item added")
               {
                  CHECK(*i == &s1);

                  AND_THEN("The iterator has not stepped")
                  {
                     CHECK(i != list.end());
                  }

                  AND_WHEN("The iterator is incremented")
                  {
                     ++i;

                     THEN("The iterator is at the end")
                     {
                        CHECK(i == list.end());
                     }
                  }
               }
            }
         }

         AND_WHEN("An iterator is created")
         {
            auto i = list.begin();

            THEN("The item count is one")
            {
               REQUIRE(list.count() == 1);

               AND_WHEN("Calling remove on the iterator")
               {
                  i = list.remove(i);

                  THEN("There is no next item")
                  {
                     REQUIRE(i == list.end());

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
         list.add(&s1);
         list.add(&s2);
         list.add(&s3);

         AND_WHEN("An iterator is created")
         {
            auto i = list.begin();

            THEN("The items can be accessed in order")
            {
               REQUIRE(i != list.end());
               CHECK(*i == &s1);
               ++i;
               REQUIRE(i != list.end());
               CHECK(*i == &s2);
               ++i;
               REQUIRE(i != list.end());
               CHECK(*i == &s3);
               ++i;
               CHECK(i == list.end());
            }
         }

         AND_WHEN("An iterator is created")
         {
            auto i = list.begin();

            AND_WHEN("Removing the second item")
            {
               ++i;
               i = list.remove(i);

               THEN("The iterator points to the third element")
               {
                  CHECK(*i == &s3);

                  AND_THEN("The item count is two")
                  {
                     CHECK(list.count() == 2);
                  }
               }
            }
         }
      }
   }
}
