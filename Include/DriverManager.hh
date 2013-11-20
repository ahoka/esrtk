#ifndef DRIVERMANAGER_HH
#define DRIVERMANAGER_HH

//#include <SpinLock.hh>

class Driver;

class DriverManager
{
public:
   static bool registerDriver(Driver* driver);
   static bool deregisterDriver(Driver* driver);

   static void probeAndInit();

private:
   // xxx should be a hierarchy of drivers instead of a flat list
   static Driver* driverList;
//   static SpinLock lock;
};

#endif
