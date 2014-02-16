#include <Driver/DriverManager.hh>
#include <Driver/Driver.hh>

#include <cstdio>

Driver* DriverManager::driverList = 0;
//SpinLock DriverManager::lock;

bool
DriverManager::registerDriver(Driver* driver)
{
   printf("DriverManager: registering driver %p\n", driver);

//   lock.enter();
   driver->next = driverList;
   driverList = driver;
//   lock.exit();

   return true;
}

bool
DriverManager::deregisterDriver(Driver* /*driver*/)
{
   return true;
}

void
DriverManager::probeAndInit()
{
   Driver* driver = driverList;
   while (driver != 0)
   {
      if (driver->probe() > 0)
      {
         driver->init();
      }
      driver = driver->next;
   }
}
