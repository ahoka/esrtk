#ifndef INTERRUPTHANDLER_HH
#define INTERRUPTHANDLER_HH

class InterruptHandler
{
public:
   InterruptHandler();
   virtual ~InterruptHandler() = default;

   InterruptHandler(const InterruptHandler&) = delete;
   InterruptHandler& operator = (const InterruptHandler&) = delete;

   virtual void handleInterrupt() = 0;
   void setNext(InterruptHandler* value);
   InterruptHandler* getNext();

private:
   InterruptHandler* next;
};

#endif
