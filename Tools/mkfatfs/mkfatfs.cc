#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
//#include <unistd.h>

#include <File.hh>

#include "../../Include/FileSystem/Fat.hh"

void
usage()
{
   fprintf(stderr, "Usage: mkfatfs <image>\n");
}

void
printFat(Fat::BootSector& fat)
{
   printf("Jump code: 0x%x 0x%x 0x%x\n", fat.jumpCode[0], fat.jumpCode[1], fat.jumpCode[2]);
   printf("OEM name: '%.8s'\n", fat.oemName);
   printf("Bytes/Sector: %u\n", (unsigned)fat.bytesPerSector);
   printf("Sector/Cluster: %u\n", (unsigned)fat.sectorsPerCluster);
   printf("Reserved sector: %u\n", (unsigned)fat.reservedSectors); // 1
   printf("FAT copies: %u\n", (unsigned)fat.fatCopies); // 2
   printf("Root dir entries: %u\n", (unsigned)fat.rootDirectoryEntries); // 512 for FAT16
   printf("Total sectors (16): %u\n", (unsigned)fat.totalSectors16); // 2880?
   printf("Media type: 0x%x\n", (unsigned)fat.mediaType); // f0 - 1.4M floppy, f8 - hdd
   printf("Sectors/FAT: %u\n", (unsigned)fat.sectorsPerFat); // 9
   printf("Sectors/Track: %u\n", (unsigned)fat.sectorsPerTrack); // 12
   printf("Heads: %u\n", (unsigned)fat.heads);
   printf("Hidden sectors: %u\n", (unsigned)fat.hiddenSectors); // 0
   printf("Total Sectors: %u\n", (unsigned)fat.totalSectors); // when totalSectors16 is zero
   printf("Logical drive: %u\n", (unsigned)fat.logicalDriveNumber); // INT13!
   printf("NT flag: %u\n", (unsigned)fat.reservedForNt); // should be 0
   printf("Extended signature: %u\n", (unsigned)fat.extendedSignature); // 0x29 (for the three below fields)
   printf("Serial: %u\n", (unsigned)fat.serialNumber);
   printf("Volume label: %.11s\n", fat.volumeLabel);
   printf("FS Type: %.8s\n", fat.fileSystemType);
}

int
main(int argc, char** argv)
{
   int error = 0;

   if (argc < 2)
   {
      usage();

      return EXIT_FAILURE;
   }

   Utility::File inFile(argv[1]);
   if (!inFile.open("r"))
   {
      fprintf(stderr, "Can't open file: %s\n", inFile.getPath().c_str());
      return inFile.getLastError();
   }

   char buffer[512];
   inFile.read(buffer, 1, sizeof(buffer));
   if (inFile.error())
   {
      fprintf(stderr, "Error reading file: %s\n", inFile.getPath().c_str());
      return inFile.getLastError();
   }

   Fat::BootSector& fat = *reinterpret_cast<Fat::BootSector*>(buffer);
   printFat(fat);

   int fatStart = fat.bytesPerSector * fat.reservedSectors;
   inFile.seek(fatStart);

   inFile.read(buffer, 1, sizeof(buffer));
   if (inFile.error())
   {
      fprintf(stderr, "Error reading file: %s\n", inFile.getPath().c_str());
      return inFile.getLastError();
   }

   for (int i = 0; i < 512 / 2; i += 2)
   {
      if (i % 8 == 0)
      {
	 printf("\n");
      }
      printf("0x%hx\t", buffer[i]);
   }
   printf("\n");

   return error;
}
