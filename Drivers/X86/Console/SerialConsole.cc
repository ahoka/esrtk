#include <X86/SerialConsole.hh>
#include <X86/IoPort.hh>

bool SerialConsole::portInitialized = false;

// init in polling mode
void
SerialConsole::init()
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
//   outb(Com0 + FifoControl, EnableFifo | FifoBytes14 | ClearTxFifo | ClearRxFifo);
   outb(Com0 + FifoControl, 0);
}

//int
//SerialConsole::getChar()
//{
//   uint8_t ch;
//
////   init();
//
//   while (inb(Com0 + LineStatus) & (1 << 5))
//   {
//      asm volatile("pause");
//   }
//
//   ch = inb(Com0 + Data);
//
//   return ch;
//}

int
SerialConsole::putChar(int ch)
{
//   init();

   while (inb(Com0 + LineStatus) & (1 << 0))
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
