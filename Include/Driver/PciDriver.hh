#ifndef PCIDRIVER_HH
#define PCIDRIVER_HH

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
   int driverInfo(const char*, ...);
   
private:
   PciDriver(const PciDriver&) = delete;
   PciDriver& operator = (const PciDriver&) = delete;

   PciDriver* next;

   friend class Pci;
};

#endif
