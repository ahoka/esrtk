#include <SystemTimer.hh>
#include <Interrupt.hh>

namespace Kernel
{
   namespace X86
   {
      class LocalApicTimer : InterruptHandler, SystemTimer
      {
      public:
         LocalApicTimer();
         ~LocalApicTimer();

         int probe();
         bool startTimer();
         bool stopTimer();

         unsigned int getFrequency();
         void delay(unsigned long ms);

         void handleInterrupt();
      };
   };
};
