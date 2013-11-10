#include <gtest/gtest.h>
#include <File.hh>

class FileTest : public ::testing::Test
{
public:
   void SetUp()
   {
   }
   void TearDown()
   {
   }
};

TEST(FileTest, testCtor)
{
   Utility::File file("TestFiles/a.txt");
}

TEST(FileTest, testOpen)
{
   Utility::File file("TestFiles/a.txt");
   bool r = file.open("r");

   ASSERT_TRUE(r);
}

TEST(FileTest, testIsOpen)
{
   Utility::File file("TestFiles/a.txt");
   bool isOpen = file.isOpen();

   ASSERT_FALSE(isOpen);

   bool r = file.open("r");
   ASSERT_TRUE(r);

   isOpen = file.isOpen();
   ASSERT_TRUE(isOpen);
}

TEST(FileTest, testOpenNonExisting)
{
   Utility::File file("TestFiles/nope");
   bool r = file.open("r");

   ASSERT_FALSE(r);
}

int
main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   
   return RUN_ALL_TESTS();
}
