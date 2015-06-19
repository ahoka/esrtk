#include <DoublyLinkedList.hh>
#include <DoublyLinkedItem.hh>

#include "catch.hpp"

class TestItem : public DoublyLinkedItem<TestItem>
{
public:

   TestItem() :
      number(0)
   {
   }

   TestItem(int num) :
      number(num)
   {
   }

   operator int()
   {
      return number;
   }

   bool operator==(const TestItem& other)
   {
      return number == other.number;
   }

private:
   int number;
};

SCENARIO("Iterators", "DoublyLinkedListTest")
{
   GIVEN("We have three items in a list")
   {
      TestItem n1 = 12;
      TestItem n2 = 23;
      TestItem n3 = 34;

      DoublyLinkedList<TestItem> list;

      list.insertLast(&n1);
      list.insertLast(&n2);
      list.insertLast(&n3);

      WHEN("We forward iterate the list")
      {
         auto r = list.range();

         THEN("We get them in order")
         {
            REQUIRE(!r.empty());
            REQUIRE(r.front().get() == n1);
            r.popFront();

            REQUIRE(!r.empty());
            REQUIRE(r.front().get() == n2);
            r.popFront();

            REQUIRE(!r.empty());
            REQUIRE(r.front().get() == n3);
            r.popFront();

            REQUIRE(r.empty());
         }
      }

      WHEN("We reverse iterate the list")
      {
         auto r = list.range();

         THEN("We get them in reverse order")
         {
            REQUIRE(!r.empty());
            REQUIRE(r.back().get() == n3);
            r.popBack();

            REQUIRE(!r.empty());
            REQUIRE(r.back().get() == n2);
            r.popBack();

            REQUIRE(!r.empty());
            REQUIRE(r.back().get() == n1);
            r.popBack();

            REQUIRE(r.empty());
         }
      }
   }
}
