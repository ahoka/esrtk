#ifndef BIOS_HH
#define BIOS_HH

// X86 BIOS definitions

#define X86_BIOS_EBDA_SEGMENT   0x000eu
#define X86_BIOS_EBDA_OFFSET    0x0040u
#define X86_BIOS_EBDA_ADDRESS   ((X86_BIOS_EBDA_SEGMENT << 4) + X86_BIOS_EBDA_OFFSET)

#define X86_BIOS_ROM_START      0x0e0000u
#define X86_BIOS_ROM_END        0x110000u
#define X86_BIOS_ROM_SIZE       0x020000u

#endif
