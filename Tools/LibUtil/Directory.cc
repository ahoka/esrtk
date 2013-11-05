#include "Directory.hh"

#include <errno.h>

using namespace Utility;

Directory::Directory(const char* path_)
   : path(path_),
     dir(0),
     lastError(0)
{
}

bool
Directory::open()
{
   dir = opendir(path.c_str());
   if (dir == 0)
   {
      lastError = errno;
      return false;
   }

   return true;
}

bool
Directory::close()
{
   if (closedir(dir) == -1)
   {
      lastError = errno;
      return false;
   }

   return true;
}

int
Directory::getLastError()
{
   return lastError;
}

