#include <X86/EarlySerial.hh>
#include <X86/IoPort.hh>
#include <X86/I8250Uart.hh>

bool EarlySerial::portInitialized = false;
bool EarlySerial::isDisabledM = true;

// init in polling mode
void
EarlySerial::init()
{
   if (portInitialized)
   {
      return;
   }
   
   outb(Com0 + InterruptEnable, 0);
   outb(Com0 + LineControl, DlabEnable);
   outb(Com0 + DivisorLow, 1);
   outb(Com0 + DivisorHigh, 0);
   outb(Com0 + LineControl, EightDataBits | NoParityBits | OneStopBit);
   outb(Com0 + FifoControl, 0);

   isDisabledM = false;
}

bool
EarlySerial::isDisabled()
{
   return isDisabledM;
}

void
EarlySerial::disable()
{
   isDisabledM = true;
}

int
EarlySerial::putChar(int ch)
{
   if (isDisabledM)
   {
      return 0;
   }

   while ((inb(Com0 + LineStatus) & EmptyTransmitterHoldingRegs) == 0)
   {
      asm volatile("pause");
   }

   outb(Com0 + Data, (uint8_t)ch);
   
   if (ch == '\n')
   {
      putChar('\r');
   }

   return 1;
}
