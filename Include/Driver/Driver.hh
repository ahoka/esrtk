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
   int driverInfo(const char*, ...) __attribute__((__format__ (__printf__, 2, 3)));
   
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
