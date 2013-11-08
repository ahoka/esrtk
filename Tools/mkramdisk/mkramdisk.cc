#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#include <File.hh>
#include <Directory.hh>
#include <DirectoryEntry.hh>

/*
  0: Magic number
  4: Number of files
  8: File Descriptor 1
     File Descriptor N
 */

enum
{
   RamdiskMagic = 0xbadbabe
};

struct  FileDescriptor
{
   uint8_t name[32];
   uint32_t offset;
   uint32_t size;
} __attribute__((packed));

void
usage()
{
   fprintf(stderr, "Usage: mkramdisk <image> <directory>\n");
}

int
copyFile(Utility::File& outputFile, std::string filename, uint32_t& size)
{
   char buffer[512];
   int r;

   Utility::File inputFile(filename);
   if (!inputFile.open())
   {
      fprintf(stderr, "Can't open input file %s: %s\n", filename.c_str(), strerror(inputFile.getLastError()));

      return inputFile.getLastError();
   }

   size = 0;
   while ((r = inputFile.read(buffer, 1, sizeof(buffer))))
   {
      size += r;
      outputFile.write(buffer, r, 1);

      if (outputFile.error())
      {
         fprintf(stderr, "Wrinting to image failed\n");
         return outputFile.getLastError();
      }
   }
      
   if (inputFile.error())
   {
      fprintf(stderr, "Reading from %s failed\n", filename.c_str());
      return inputFile.getLastError();
   }

   return 0;
}

int
main(int argc, char** argv)
{
   int error = 0;

   if (argc < 3)
   {
      usage();

      return EXIT_FAILURE;
   }

   Utility::File outFile(argv[1]);
   if (!outFile.open())
   {
      fprintf(stderr, "Can't open file: %s\n", outFile.getPath().c_str());
      return outFile.getLastError();
   }

   Utility::Directory dir(argv[2]);

   Utility::File tmpContentFile("content.tmp");
   if (!tmpContentFile.open())
   {
      fprintf(stderr, "Can't open file: %s\n", tmpContentFile.getPath().c_str());
      return tmpContentFile.getLastError();
   }

   Utility::File tmpHeaderFile("header.tmp");
   if (!tmpHeaderFile.open())
   {
      fprintf(stderr, "Can't open file: %s\n", tmpHeaderFile.getPath().c_str());
      return tmpHeaderFile.getLastError();
   }

   // if (chdir(argv[2]) == -1)
   // {
   //    perror("Can't chdir to input directory");
   //    error = errno;

   //    goto out3;
   // }

   uint32_t offset = 0;
   char namebuf[32];
//   struct dirent* dent;
//   while ((dent = readdir(dir)) != NULL)
   for (auto entry : dir)
   {
//      if (strcmp(dent->d_name, ".") && strcmp(dent->d_name, ".."))
      printf("File: %s\n", entry.getName().c_str());
      if (entry.getName() != "." && entry.getName() != "..")
      {
         uint32_t size;
         error = copyFile(tmpContentFile, entry.getPath(), size);
         if (error != 0)
         {
            return error;
         }

         offset += size;
         printf("ramdisk:%s (%lu:%lu)\n", entry.getName().c_str(), (unsigned long )offset, (unsigned long )size);

         memset(namebuf, 0, sizeof(namebuf));
         strcpy(namebuf, entry.getName().c_str());

         tmpHeaderFile.write(namebuf, 32, 1);
         tmpHeaderFile.write(&offset, sizeof(offset), 1);
         tmpHeaderFile.write(&size, sizeof(size), 1);
      }
   }

   // concatenate
   char buffer[128];
   int r;

   tmpHeaderFile.rewind();
   while ((r = tmpHeaderFile.read(buffer, 1, sizeof(buffer))))
   {
      outFile.write(buffer, r, 1);
   }
   
   tmpContentFile.rewind();
   while ((r = tmpContentFile.read(buffer, 1, sizeof(buffer))))
   {
      outFile.write(buffer, r, 1);
   }

   return error;
}
