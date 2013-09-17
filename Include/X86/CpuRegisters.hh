#ifndef SYSTEMCONTROL_HH
#define SYSTEMCONTROL_HH

#include <Register.hh>

extern "C" void printCpuFeatures();

namespace CR0
{
   enum CR0
   {
      PE = BIT(0),  // Protection enabled
      MP = BIT(1),  // Monitor Coprocessor
      EM = BIT(2),  // Emulation
      TS = BIT(3),  // Task Switched
      ET = BIT(4),  // Extension Type (Read Only)
      NE = BIT(5),  // Numeric Error
      WP = BIT(16), // Write Protect
      AM = BIT(18), // Alignment Mask
      NW = BIT(29), // Not Writethrough
      CD = BIT(30), // Cache Disable
      PG = BIT(31)  // Paging
   };
}

namespace CR4
{
   enum CR4
   {
      VME = BIT(0),         // Virtual-8086 Mode Extensions
      PVI = BIT(1),	    // Protected-Mode Virtual Interrupts
      TSD = BIT(2),	    // Time Stamp Disable
      DE = BIT(3),	    // Debugging Extensions
      PSE = BIT(4),	    // Page Size Extensions
      PAE = BIT(5),	    // Physical-Address Extension
      MCE = BIT(6),	    // Machine Check Enable
      PGE = BIT(7),	    // Page Global Enable
      PCE = BIT(8),	    // Performance-Monitoring Counter Enable
      OSFXSR = BIT(9),	    // Operating System FXSAVE/FXRSTOR Support
      OSXMMEXCPT = BIT(10), // Operating System Unmasked Exception Support
      FSGBASE = BIT(16),    // Enable RDFSBASE, RDGSBASE, WRFSBASE, and WRGSBASE instructions
      OSXSAVE = BIT(18)	    // XSAVE and Processor Extended States Enable Bit
   };
}

namespace CR3PAE
{
   enum CR3
   {
      PWT = BIT(3),
      PCD = BIT(4),
   };
}

// ??
enum
{
   Cr3PaeAddressShift = 12,
   Cr3AddressShift = 5
};

namespace PAE
{
   namespace PDPE
   {
      enum
      {
	 P = BIT(0),
	 PWT = BIT(3),
	 PCD = BIT(4)
      };
   }
   namespace PDE
   {
      enum
      {
	 P = BIT(0),
	 PWT = BIT(3),
	 PCD = BIT(4)
      };
   }

}

namespace CPUID
{
   enum CPUID_0x1
   {
      FPU = BIT(0),
      VME = BIT(1),
      DE = BIT(2),
      PSE = BIT(3),
      TSC = BIT(4),
      MSR = BIT(5),
      PAE = BIT(6),
      MCE = BIT(7),
      CX8 = BIT(8),
      APIC = BIT(9)
   };
}

#endif
