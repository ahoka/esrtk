#ifndef PCIDRIVER_HH
#define PCIDRIVER_HH

#include <Kernel/Pci.hh>

#include <cstdint>
// TODO common-ish interface with root drivers?

class PciDriver
{
public:
   PciDriver();
   virtual ~PciDriver();

   virtual int probe(uint8_t bus, uint8_t device, uint8_t function) = 0;
   virtual bool init(uint8_t bus, uint8_t device, uint8_t function) = 0;
   virtual bool finalize() = 0;
   virtual const char* name() const = 0;

protected:
   int driverInfo(const char*, ...) __attribute__((__format__ (__printf__, 2, 3)));
   
private:
   PciDriver(const PciDriver&) = delete;
   PciDriver& operator = (const PciDriver&) = delete;

   PciDriver* next;

   friend void Pci::registerDriver(PciDriver* driver);
   friend bool Pci::probeAndAttach(uint8_t bus, uint8_t device, uint8_t function);
};

#endif
