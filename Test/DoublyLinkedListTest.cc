#include <DoublyLinkedList.hh>
#include <DoublyLinkedItem.hh>
#include <iostream>

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

//private:
   int number;
};

class DoublyLinkedListTest
{
public:
   void test()
   {
      TestItem n1 = 12;
      TestItem n2 = 23;
      TestItem n3 = 34;
      
      DoublyLinkedList<TestItem> list;
      
      list.insertLast(&n1);
      list.insertLast(&n2);
      list.insertLast(&n3);

      std::cout << "Iterating forward using range:" << std::endl;
      for (auto r = list.range(); !r.empty(); r.popFront())
      {
         TestItem& item = r.front();
         std::cout << item << std::endl;
      }

      std::cout << "Iterating backward using range:" << std::endl;
      for (auto r = list.range(); !r.empty(); r.popBack())
      {
         TestItem& item = r.back();
         std::cout << item << std::endl;         
      }

      std::cout << "Iterating forward using iterator:" << std::endl;
      for (auto item : list)
      {
         std::cout << item << std::endl;
      }
   }
};

int
main()
{
   DoublyLinkedListTest test;

   test.test();

   return 0;
}
