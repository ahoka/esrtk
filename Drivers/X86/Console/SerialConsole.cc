#include <X86/IoPort.hh>
#include <X86/I8250Uart.hh>
#include <StdioSupport.hh>
#include <X86/EarlySerial.hh>

#include <Debug.hh>

#include <Driver/Driver.hh>
#include <Driver/Console.hh>
#include <Interrupt.hh>

#include <cstdio>
#include <cstdint>

namespace {
   class SerialConsole : Driver, InterruptHandler, Console
   {
   public:
      SerialConsole();
      ~SerialConsole();

      int probe();
      bool init();
      bool finalize();
      const char* name() const;

      virtual void putChar(char ch);

      // this should be interruptHandler()
      void handleInterrupt();
   };
}

SerialConsole::SerialConsole()
{
   Debug::info("SerialConsole::SerialConsole()\n");
}

SerialConsole::~SerialConsole()
{
   Debug::info("SerialConsole::~SerialConsole()\n");
}

int
SerialConsole::probe()
{
   return 1;
}

bool
SerialConsole::init()
{
   // disable early serial driver
   //
   EarlySerial::disable();

   outb(Com0 + InterruptEnable, 1);
   outb(Com0 + LineControl, DlabEnable);
   outb(Com0 + DivisorLow, 1);
   outb(Com0 + DivisorHigh, 0);
   outb(Com0 + LineControl, EightDataBits | NoParityBits | OneStopBit);
//   outb(Com0 + FifoControl, EnableFifo | FifoBytes14 | ClearTxFifo | ClearRxFifo);
   outb(Com0 + FifoControl, 0);

   bool success = Interrupt::registerHandler(Com0Irq, this);
   Interrupt::enableInterrupt(Com0Irq);

   driverInfo("initalized\n");

   Console::enable();

   return success;
}

bool
SerialConsole::finalize()
{
   Interrupt::disableInterrupt(Com0Irq);
   bool success = Interrupt::deregisterHandler(Com0Irq, this);

   return success;
}

const char*
SerialConsole::name() const
{
   return "SerialConsole";
}

void
SerialConsole::handleInterrupt()
{
   uint8_t iir = inb(Com0 + InterruptIdentification);

   // check if we need to read something
   //
   if (iir & 0x4)
   {
      // reading data resets this flag
      //
      char c = inb(Com0 + Data);

      if (c == '\r')
      {
         console_feed('\n');
      }
      else
      {
         console_feed(c);
      }
   }
}

void
SerialConsole::putChar(char ch)
{
   while ((inb(Com0 + LineStatus) & EmptyTransmitterHoldingRegs) == 0)
   {
      asm volatile("pause");
   }

   outb(Com0 + Data, (uint8_t)ch);
   
   if (ch == '\n')
   {
      putChar('\r');
   }
}

static SerialConsole serialConsole;
