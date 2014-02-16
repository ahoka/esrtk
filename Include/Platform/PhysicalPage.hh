#ifndef PHYSICALPAGE_HH
#define PHYSICALPAGE_HH

class PhysicalPage
{
public:
   enum
   {
      // extreme value
      Invalid = ~0UL
   };

   PhysicalPage()
      : prev(this),
	next(this),
	address(Invalid)
   {
      // empty
   }

   uintptr_t getAddress()
   {
      return address & AddressMask;
   }

   // this wont preserve color
   void setAddress(uintptr_t addr)
   {
      address = addr;
   }

   void setColor(uint8_t color)
   {
      address &= AddressMask;
      address |= color;
   }

   uint8_t getColor()
   {
      return static_cast<uint8_t>(address & ColorMask);
   }

   bool isValid()
   {
      return address == Invalid;
   }

   bool isInvalid()
   {
      return !isValid();
   }

private:
   friend class PageCluster;

   PhysicalPage* prev;
   PhysicalPage* next;
   uintptr_t address;

   enum
   {
      AddressMask = ~PageMask,
      ColorMask = PageMask
   };
};

#endif
