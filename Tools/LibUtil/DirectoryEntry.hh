#ifndef __DIRECTORYENTRY_HH__
#define __DIRECTORYENTRY_HH__

#include <string>
#include <dirent.h>

namespace Utility
{

class DirectoryEntry
{
public:
   DirectoryEntry(struct dirent* dentry);

   std::string getName();
   bool isFile();
   bool isDirectory();

public:
   struct dirent* dent;
};

}

#endif
