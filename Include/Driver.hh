#ifndef DRIVER_HH
#define DRIVER_HH

class Driver
{
public:
   Driver();
   virtual ~Driver();
   
   virtual int probe() = 0;
   virtual bool init() = 0;
   virtual bool finalize() = 0;
   
private:
   enum class State
   {
      Uninitalized,
      Initialized
   };
   
   State state;
   Driver* next;

   friend class DriverManager;
};

#endif
