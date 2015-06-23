#include "catch.hpp"

//#define KASSERT REQUIRE

#include <Segment.hh>

#include <cstring>

#define SEGMENTSIZE 4096

SCENARIO("Memory segment representation", "[kernelheap]")
{
   GIVEN("A memory segment")
   {
      char* memorySegment = new char[SEGMENTSIZE];
      memset(memorySegment, 0x55, SEGMENTSIZE);
      
      WHEN("Constructing a segment header for the segment")
      {
         Segment* segment = new(memorySegment) Segment();

         THEN("The size is zero")
         {
            CHECK(segment->getSize() == 0);
         }
            
         THEN("The segment is marked as unallocated")
         {
            CHECK(segment->isAllocated() == false);
         }

         AND_WHEN("Updating the checksum")
         {
            segment->updateChecksum();

            THEN("The checksum is valid")
            {
               CHECK(segment->verifyChecksum() == true);
            }

            AND_WHEN("The segment is mutated")
            {
               segment->setSize(1110);

               THEN("The checksum is no longer valid")
               {
                  CHECK(segment->verifyChecksum() == false);
               }
            }
         }
      }
   }
}
