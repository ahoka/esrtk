#if 0
#include "TempFile.hh"

#include <cstdio>
#include <cstdlib>
#include <errno.h>

using namespace Utility;

std::string
TempFile::createTempFile(std::string filename, std::string directory = "/tmp")
{
   if (filename.find("/"))
   {
      // XXX
      return std::string();
   }

   char tempFileName[PATH_MAX];
   snprintf(tempFileName, sizeof(tempFileName), "%s/%s-XXXXXX", directory.data(), filename.data());

   int tempFile = mkstemp(path.c_str());
   if (tmpfd_header == -1)
   {
      // XXX
      return std::string();
   }

   return path;
}

TempFile::TempFile(std::string filename)
   : File("")
{
   
}

#endif
