#ifndef I8250UART_HH
#define I8250UART_HH

enum
{
   Com0 = 0x3f8,
   Com1 = 0x2f8,
   Com2 = 0x3e8,
   Com3 = 0x2e8
};

enum
{
   Com0Irq = 4,
   Com1Irq = 3,
   Com2Irq = 4,
   Com3Irq = 3
};

enum
{
   Data = 0,
   InterruptEnable = 1,
   DivisorLow = 0,
   DivisorHigh = 1,
   InterruptIdentification = 2,
   FifoControl = 2,
   LineControl = 3,
   ModemControl = 4,
   LineStatus = 5,
   ModemStatus = 6,
   Scratch = 7
};

enum DataBits
{
   FiveDataBits = 0,
   SixDataBits = 1,
   SevenDataBits = 2,
   EightDataBits = 3
};

enum StopBits
{
   OneStopBit = 0 << 2,
   TwoStopBits = 1 << 2
};

enum ParityBits
{
   NoParityBits = 0 << 3
   // TODO
};

enum LineControlBits
{
   DlabEnable = 1 << 7
};

enum FifoBits
{
   EnableFifo = 1,
   ClearRxFifo = 1 << 1,
   ClearTxFifo = 1 << 2,
   Enable64ByteFifo = 1 << 5,
   FifoBytes1 = 0 << 6,
   FifoBytes4 = 1 << 6,
   FifoBytes8 = 2 << 6,
   FifoBytes14 = 3 << 6,
};

enum LineStatusRegisterBits
{
   FifoError = 1 << 7,
   EmptyDataHoldingRegs = 1 << 6,
   EmptyTransmitterHoldingRegs = 1 << 5,
   BreakInterrupt = 1 << 4,
   FramingError = 1 << 3,
   ParityError = 1 << 2,
   OverrunError = 1 << 1,
   DataReady = 1 << 0
};

#endif
