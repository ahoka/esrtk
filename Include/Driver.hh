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


class DriverManager
{
public:
   static bool registerDriver(Driver* driver);
   static bool deregisterDriver(Driver* driver);

   static void probeAndInit();

private:
   // xxx should be a hierarchy of drivers instead of a flat list
   static Driver* driverList;
};
