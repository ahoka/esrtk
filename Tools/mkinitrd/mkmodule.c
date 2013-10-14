#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct
{
   uint32_t magic;
   uint32_t type;
   uint32_t size;
   uint32_t checksum;
} __attribute__((packed)) ModuleHeader;

void
usage()
{
   fprintf(stderr, "Usage: mkinitrd <image>\n");
}

int
main(int argc, char** argv)
{
   char buffer[512];
   size_t module_size = 0;
   int r;

   if (argc < 3)
   {
      usage();

      return EXIT_FAILURE;
   }

   FILE* ifile = fopen(argv[1], "r");

   if (ifile == NULL)
   {
      perror("Can't open input file");

      return EXIT_FAILURE;
   }

   FILE* ofile = fopen(argv[2], "w");

   if (ofile == NULL)
   {
      perror("Can't open output file");
      fclose(ifile);

      return EXIT_FAILURE;
   }

   while ((r = fread(buffer, 1, sizeof(buffer), ifile)))
   {
      module_size += r;
   }

   if (ferror(ifile))
   {
      perror("Error while reading input file");

      fclose(ifile);
      fclose(ofile);

      return EXIT_FAILURE;
   }

   fprintf(stderr, "Input file size: %zu\n", module_size);

   ModuleHeader header;

   header.magic = 0x30646f4d;
   header.type = 0;
   header.size = module_size;
   header.checksum = 0;

   fwrite(&header, sizeof(header), 1, ofile);

   r = fseek(ifile, 0L, SEEK_SET);
   if (r == -1)
   {
      fclose(ofile);
      fclose(ifile);

      return EXIT_FAILURE;
   }

   while ((r = fread(buffer, 1, sizeof(buffer), ifile)))
   {
      fwrite(buffer, r, 1, ofile);
   }

   if (ferror(ofile) || ferror(ifile))
   {
      perror("Copying image failed");
      fclose(ofile);
      fclose(ifile);
   
      return EXIT_FAILURE;
   }

   fclose(ofile);
   fclose(ifile);

   return EXIT_SUCCESS;
}
