#include <SinglyLinkedList.hh>
#include <SinglyLinkedItem.hh>
#include <iostream>

class TestItem : public SinglyLinkedItem<TestItem>
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

//private:
   int number;
};

class SinglyLinkedListTest
{
public:
   void test()
   {
      TestItem n1 = 12;
      TestItem n2 = 23;
      TestItem n3 = 34;

      SinglyLinkedList<TestItem> list;

      list.insertFirst(&n1);
      list.insertFirst(&n2);
      list.insertFirst(&n3);

      std::cout << "Iterating forward:" << std::endl;
      for (auto r = list.range(); !r.empty(); r.popFront())
      {
         TestItem& item = r.front();
         std::cout << item << std::endl;
      }

      SinglyLinkedList<TestItem> list2;

      list2.insertLast(&n1);
      list2.insertLast(&n2);
      list2.insertLast(&n3);

      std::cout << "Iterating forward:" << std::endl;
      for (auto r = list2.range(); !r.empty(); r.popFront())
      {
         TestItem& item = r.front();
         std::cout << item << std::endl;
      }
   }
};
