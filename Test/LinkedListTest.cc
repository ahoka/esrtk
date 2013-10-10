#include <LinkedList.hh>
//#include <cstdio>
#include <iostream>

class TestItem : public LinkedListItem<TestItem>
{
public:

   TestItem() : number(0)
   {
   }

   TestItem(int num) : number(num)
   {
   }

   operator int()
   {
      return number;
   }

//private:
   int number;
};

class LinkedListTest
{
public:
   void test()
   {
      TestItem n1 = 12;
      TestItem n2 = 23;
      TestItem n3 = 34;
      
      LinkedList<TestItem> list;
      
      list.insertLast(&n1);
      list.insertLast(&n2);
      list.insertLast(&n3);

      for (auto r = list.range(); !r.empty(); r.popFront())
      {
//         std::cout << r.front() << std::endl;
         TestItem& item = r.front();
         std::cout << item << std::endl;

//         printf("%d\n", item.number);
      }
      
      // for (auto iterator = list.getIterator();
      //      *iterator != list.end();
      //      ++iterator)
      // {
      //    printf("%d\n", (*iterator).number);
      // }
   }
};

int
main()
{
   LinkedListTest test;

   test.test();

   // int i = 1;
   // RefReference iref(i);
   // RefReference tref(test);

   return 0;
}
