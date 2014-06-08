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
   virtual const char* name() const = 0;

protected:
   int driverInfo(const char*, ...);
   
private:
   Driver(const Driver&) = delete;
   Driver& operator = (const Driver&) = delete;

   // XXX hide this
   enum class State
   {
      Uninitalized,
      Initialized
   };
   
   State stateM;
   Driver* nextM;

   friend class DriverManager;
};

#endif
