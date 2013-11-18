#include <X86/IoPort.hh>

#include <SystemTypes.hh>

//#include <Interrupt.hh>
#include <InterruptController.hh>
#include <Register.hh>
#include <Debug.hh>

#include <cstdio>

class Pic : InterruptController
{
   enum Ports
   {
      MasterCommand = 0x20,
      MasterData = 0x21,
      SlaveCommand = 0xa0,
      SlaveData = 0xa1
   };

   enum Icw1
   {
      Icw4Needed = BIT(0),
      InitConfiguration = BIT(4)
   };

   enum Icw4
   {
      X86Mode = BIT(0)
   };

   enum Commands
   {
      EndOfInterrupt = 0x20
   };

   enum VectorOffsets
   {
      MasterOffset = 32,
      SlaveOffset = 40
   };

   void init()
   {
//      uint8_t masterMask = inb(MasterData);
//      uint8_t slaveMask = inb(SlaveData);

      // setup the master controller
      //

      // start configuration mode
      outb(MasterCommand, Icw4Needed | InitConfiguration);
      // set interrupt offset
      outb(MasterData, MasterOffset);
      // irq2 is connected to the slave controller
      outb(MasterData, BIT(2));
      // set 8086 mode
      outb(MasterData, X86Mode);
      
      // setup the slave controller
      //
      
      // start configuration mode
      outb(SlaveCommand, Icw4Needed | InitConfiguration);
      // set interrupt offset
      outb(SlaveData, SlaveOffset);
      // we connect to irq2
      outb(SlaveData, 2);
      // set 8086 mode
      outb(SlaveData, X86Mode);

//      outb(MasterData, masterMask);
//      outb(SlaveData, slaveMask);

      outb(MasterData, 0xff);
      outb(SlaveData, 0xff);

//      printf("Original interrupt masks: 0x%x 0x%x\n", masterMask, slaveMask);

      asm volatile("sti");
   }

public:
   Pic()
   {
      printf("Initializing PIC\n");
      init();
   }

   void endOfInterrupt(irq_t irq);
   void enableInterrupt(irq_t irq);
   void disableInterrupt(irq_t irq);
};

void Pic::endOfInterrupt(irq_t irq)
{
   KASSERT(irq >= 0 && irq < 16);

//   printf("Debug: IRQ%d EOI\n", irq);

   if (irq > 7)
   {
      outb(SlaveCommand, EndOfInterrupt);
   }
   
   outb(MasterCommand, EndOfInterrupt);
}

void Pic::enableInterrupt(irq_t irq)
{
   KASSERT(irq >= 0 && irq < 16);

   if (irq > 7)
   {
      irq -= 8;
      int mask = ~(0x1 << irq);

      uint8_t original = inb(SlaveData);
      outb(SlaveData, original & (uint8_t )mask);
   }
   else
   {
      int mask = ~(0x1 << irq);

      uint8_t original = inb(MasterData);
      outb(MasterData, original & (uint8_t )mask);
   }
}

void Pic::disableInterrupt(irq_t irq)
{
   KASSERT(irq >= 0 && irq < 16);

   if (irq > 7)
   {
      irq -= 8;
      int mask = 0x1 << irq;

      uint8_t original = inb(SlaveData);
      outb(SlaveData, original | (uint8_t )mask);
   }
   else
   {
      int mask = 0x1 << irq;

      uint8_t original = inb(MasterData);
      outb(MasterData, original | (uint8_t )mask);
   }
}

static Pic pic;
