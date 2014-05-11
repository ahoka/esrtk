#include <Driver/Driver.hh>
#include <Interrupt.hh>

class Keyboard : Driver, InterruptHandler
{
public:
   Keyboard();
   ~Keyboard();

   int probe();
   bool init();
   bool finalize();
   const char* name() const;

   // this should be interruptHandler()
   void handleInterrupt();
};
