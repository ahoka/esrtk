#ifndef __DIRECTORYENTRY_HH__
#define __DIRECTORYENTRY_HH__

#include <string>
#include <dirent.h>

namespace Utility
{

class DirectoryEntry
{
public:
   DirectoryEntry(dirent de, std::string parent);

   std::string getPath();
   std::string getName();
   bool isFile();
   bool isDirectory();

public:
   struct dirent dent;
   std::string directory;
};

}

#endif
