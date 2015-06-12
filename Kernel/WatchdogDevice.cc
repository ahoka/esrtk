#include <Kernel/WatchdogDevice.hh>
#include <spinlock.h>

namespace
{
   WatchdogDevice* devices = 0;
   spinlock_softirq_t lock = SPINLOCK_SOFTIRQ_STATIC_INITIALIZER;
}

bool
WatchdogDevice::registerWatchdog()
{
   spinlock_softirq_enter(&lock);

   this->nextWatchdogDeviceM = devices;
   devices = this;

   spinlock_softirq_exit(&lock);

   return true;
}

bool
WatchdogDevice::unregisterWatchdog()
{
   spinlock_softirq_enter(&lock);

   WatchdogDevice* previous = 0;
   WatchdogDevice* device = devices;
   while (device != 0)
   {
      if (device == this)
      {
         if (previous != 0)
         {
            previous->nextWatchdogDeviceM = device->nextWatchdogDeviceM;
         }

         spinlock_softirq_exit(&lock);
         return true;
      }

      previous = device;
      device = this->nextWatchdogDeviceM;
   }

   spinlock_softirq_exit(&lock);

   return false;
}

void
Watchdog::kick()
{
   spinlock_softirq_enter(&lock);

   WatchdogDevice* device = devices;
   while (device != 0)
   {
      device->kick();
      device = device->nextWatchdogDeviceM;
   }

   spinlock_softirq_exit(&lock);
}

