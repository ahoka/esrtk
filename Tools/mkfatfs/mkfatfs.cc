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
printBootSector(Fat::BootSector& fat)
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

void
fatTimeToHms(uint16_t time, int& h, int& m, int& s)
{
   s = (time & 0x1f) * 2;
   m = (time >> 5) & 0x3f;
   h = (time >> 11) & 0x1f;
}

void
fatDateToYmd(uint16_t date, int& y, int& m, int& d)
{
   d = date & 0x1f;
   m = (date >> 5) & 0xf;
   y = 1980 + ((date >> 9) & 0x7f);
}

void
printDirectoryEntry(Fat::DirectoryEntry& file)
{
   // LFN entry
   if (file.attributes == 0xf)
   {
      return;
   }

   if (file.fileName[0] == 0xe5)
   {
      printf("File is deleted\n");
   }
   else
   {
      printf("File name: %.8s %.3s\n", file.fileName, file.fileName + 8); // empty extension
   }
   printf("Attributes: 0x%x\n", file.attributes); // ro:hidden:sysfile:volumelabel:subir:archive:un:un
   int a, b, c;
   fatTimeToHms(file.time, a, b, c);
   printf("Time: %.2u:%.2u:%.2u\n", a, b, c);
   fatDateToYmd(file.date, a, b, c);
   printf("Date: %.2u-%.2u-%.2u\n", a, b, c);
   printf("Starting cluster: 0x%x\n", file.startingCluster);
   printf("File size: %u\n", file.fileSize);
}

#define CLUSTER(x) (dataStart + (x) * fat.sectorsPerCluster * fat.bytesPerSector)
#define FAT(x) (fatStart + (x) * 2)

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
   printBootSector(fat);

   int fatStart = fat.bytesPerSector * fat.reservedSectors;
   int fatSize = fat.bytesPerSector * fat.fatCopies * fat.sectorsPerFat;

   int rootDirStart = fatStart + (fat.fatCopies * fat.sectorsPerFat * fat.bytesPerSector);
   int rootDirSize = fat.rootDirectoryEntries * 32;

   char* rootDir = new char[rootDirSize];

   inFile.seek(rootDirStart);
   inFile.read(rootDir, 1, rootDirSize);
   if (inFile.error())
   {
      fprintf(stderr, "Error reading file: %s\n", inFile.getPath().c_str());
      return inFile.getLastError();
   }

   Fat::DirectoryEntry* root = reinterpret_cast<Fat::DirectoryEntry*>(rootDir);
   for (int i = 0; i < fat.rootDirectoryEntries; i++)
   {   
      printf("\n");
      if (root->fileName[0] == 0)
      {
	 break;
      }
      printDirectoryEntry(*root);
      if ((root->attributes & Fat::Directory) == 0 &&
	  (root->attributes & Fat::Hidden) == 0)
      {
      // 	 for (uint16_t cluster = root->startingCluster;
      // 	      (cluster > 0x0001 && cluster < 0xfff7);
      // 	      cluster = FAT(cluster))
      // 	 {
      // 	    printf("cluster: 0x%x\n", cluster);
      // 	 }
      // }
      root++;
   }

   int dataStart = rootDirStart + fat.rootDirectoryEntries * 32;
   int dataSize = (fat.totalSectors16 ? fat.totalSectors16 : fat.totalSectors) * fat.bytesPerSector - dataStart;;

   printf("Root dir entries: %u\n", fat.rootDirectoryEntries);
   printf("FAT: %u (%u)\n", fatStart, fatSize);
   printf("Root Dir: %u (%u)\n", rootDirStart, rootDirSize);
   printf("Data: %u (%u)\n", dataStart, dataSize);

   delete[] rootDir;

   return error;
}
