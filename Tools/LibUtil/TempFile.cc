#include "TempFile.hh"

#include <cstdio>

TempFile::TempFile()
{
   char tmpfilename[PATH_MAX];
   snprintf(tmpfilename, sizeof(tmpfilename), "%s/tmp-XXXXXX", "/tmp");
   int tmpfd_header = mkstemp(tmpfilename);
   if (tmpfd_header == -1)
   {
      perror("Can't create tmp file");
      return EXIT_FAILURE;
   }
}

