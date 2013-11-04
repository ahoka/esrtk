#ifdef __linux__
# define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

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

typedef struct
{
   uint8_t name[32];
   uint32_t offset;
   uint32_t size;
} __attribute__((packed)) FileDescriptor;

void
usage()
{
   fprintf(stderr, "Usage: mkramdisk <image> <directory>\n");
}

int
copy_file(FILE *ofile, const char *filename, uint32_t *size)
{
   char buffer[512];
   int r;

   FILE* ifile = fopen(filename, "r");
   if (ifile == NULL)
   {
      perror("Can't open input file");

      return errno;
   }

   *size = 0;
   while ((r = fread(buffer, 1, sizeof(buffer), ifile)))
   {
      *size += r;
      fwrite(buffer, r, 1, ofile);
   }

   if (ferror(ofile) || ferror(ifile))
   {
      perror("Copying file to image failed");
      fclose(ifile);
   
      return errno;
   }

   fclose(ifile);

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

   FILE* ofile = fopen(argv[1], "w");

   if (ofile == NULL)
   {
      perror("Can't open output file");
      error = errno;

      goto out;
   }

   DIR* dir = opendir(argv[2]);
   if (dir == NULL)
   {
      perror("Can't open input directory");
      error = errno;

      goto out2;
   }

   char tmpfilename[PATH_MAX];
   snprintf(tmpfilename, sizeof(tmpfilename), "%s/mkramdisk-superblock-XXXXXX", "/tmp");
   int tmpfd_header = mkstemp(tmpfilename);
   if (tmpfd_header == -1)
   {
      perror("Can't create tmp file");
      error = errno;
      
      goto out3;
   }

   snprintf(tmpfilename, sizeof(tmpfilename), "%s/mkramdisk-content-XXXXXX", "/tmp");
   int tmpfd_content = mkstemp(tmpfilename);
   if (tmpfd_content == -1)
   {
      perror("Can't create tmp file");
      error = errno;
      
      goto out3;
   }

   FILE *tmpfile_header = fdopen(tmpfd_header, "r+");
   if (tmpfile_header == NULL)
   {
      perror("Can't open tmp file");
      error = errno;

      goto out3;
   }

   FILE *tmpfile_content = fdopen(tmpfd_content, "r+");
   if (tmpfile_content == NULL)
   {
      perror("Can't open tmp file");
      error = errno;

      goto out3;
   }

   if (chdir(argv[2]) == -1)
   {
      perror("Can't chdir to input directory");
      error = errno;

      goto out3;
   }

   uint32_t offset = 0;
   char namebuf[32];
   struct dirent* dent;
   while ((dent = readdir(dir)) != NULL)
   {
      if (strcmp(dent->d_name, ".") && strcmp(dent->d_name, ".."))
      {
         uint32_t size;
         error = copy_file(tmpfile_content, dent->d_name, &size);
         if (error != 0)
         {
            break;
         }

         offset += size;
         printf("ramdisk:%s (%lu:%lu)\n", dent->d_name, (unsigned long )offset, (unsigned long )size);

         memset(namebuf, 0, sizeof(namebuf));
         strcpy(namebuf, dent->d_name);

         fwrite(namebuf, sizeof(namebuf), 1, tmpfile_header);
         fwrite(&offset, sizeof(offset), 1, tmpfile_header);
         fwrite(&size, sizeof(size), 1, tmpfile_header);
      }
   }

   // concatenate
   char buffer[128];
   int r;
   fseek(tmpfile_header, 0L, SEEK_SET);
   while ((r = fread(buffer, 1, sizeof(buffer), tmpfile_header)))
   {
      fwrite(buffer, r, 1, ofile);
   }
   
   fseek(tmpfile_content, 0L, SEEK_SET);
   while ((r = fread(buffer, 1, sizeof(buffer), tmpfile_content)))
   {
      fwrite(buffer, r, 1, ofile);
   }

   fclose(tmpfile_header);
   fclose(tmpfile_content);
  out3:
   closedir(dir);
  out2:
   fclose(ofile);
  out:
   return error;
}
