#include <LinkedList.hh>
#include <cstdio>

class TestItem : public LinkedListItem<TestItem>
{
public:

   TestItem() : number(0)
   {
   }

   TestItem(int num) : number(num)
   {
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
      
      for (auto iterator = list.getIterator();
	   *iterator != list.end();
	   ++iterator)
      {
	 printf("%d\n", (*iterator).number);
      }
   }
};

int
main()
{
   LinkedListTest test;

   test.test();

   return 0;
}
