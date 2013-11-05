#ifndef __DIRECTORY_HH__
#define __DIRECTORY_HH__

#include "File.hh"
#include <string>
#include <cstdio>

#include <dirent.h>

namespace Utility
{

class Directory
{
public:
   Directory(const char* path);
   bool open();
   bool close();

   int getLastError();
private:
   std::string path;
   DIR* dir;
   int lastError;
};

}

#endif

