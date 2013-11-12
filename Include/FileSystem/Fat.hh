#ifndef __FAT_HH__
#define __FAT_HH__

#include <cstdint>

namespace Fat
{

struct BootSector
{
   uint8_t jumpCode[3];
   uint8_t oemName[8]; // "MSWIN4.1"
   uint16_t bytesPerSector; // _512_, 1024, 2048, 4096
   uint8_t sectorsPerCluster; // _1_, 2, 4, 8, 16, 32, 64, 128
   uint16_t reservedSectors; // 1
   uint8_t fatCopies; // 2
   uint16_t rootDirectoryEntries; // 512 for FAT16
   uint16_t totalSectors16; // 2880?
   uint8_t mediaType; // f0 - 1.4M floppy, f8 - hdd
   uint16_t sectorsPerFat; // 9
   uint16_t sectorsPerTrack; // 12
   uint16_t heads;
   uint32_t hiddenSectors; // 0
   uint32_t totalSectors; // when totalSectors16 is zero
   uint8_t logicalDriveNumber; // INT13!
   uint8_t reservedForNt; // should be 0
   uint8_t extendedSignature; // 0x29 (for the three below fields)
   uint32_t serialNumber;
   uint8_t volumeLabel[11]; // "NO NAME   "
   uint8_t fileSystemType[8]; // "FAT16   "
} __attribute__((packed));

struct DirectoryEntry
{
   uint8_t fileName[11]; // 8+3
   uint8_t attributes; // ro:hidden:sysfile:volumelabel:subir:archive:un:un
   uint8_t _reserved_[10];
   uint16_t time;
   uint16_t date;
   uint16_t startingCluster;
   uint32_t fileSize;
} __attribute__((packed));

enum Attributes
{
   ReadOnly = 1 << 0,
   Hidden = 1 << 1,
   System = 1 << 2,
   VolumeName = 1 << 3,
   Directory = 1 << 4,
   Archive = 1 << 5
};

};

#endif

