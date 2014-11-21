#ifndef MUTEX_HH
#define MUTEX_HH

class Mutex
{
public:
   Mutex() = default;
   virtual ~Mutex() = default;

   virtual void enter() = 0;
   virtual bool tryEnter() = 0;
   virtual void exit() = 0;
};

#endif
