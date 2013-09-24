#include <DriverManager.hh>
#include <Driver.hh>

#include <cstdio>

Driver* DriverManager::driverList = 0;

bool
DriverManager::registerDriver(Driver* driver)
{
   printf("DriverManager: registering driver %p\n", driver);

   driver->next = driverList;
   driverList = driver;

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
