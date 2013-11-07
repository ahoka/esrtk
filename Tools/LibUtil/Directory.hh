#ifndef __DIRECTORY_HH__
#define __DIRECTORY_HH__

#include "File.hh"
#include <string>
#include <cstdio>

//#include <dirent.h>
struct dirent;
#include "DirectoryEntry.hh"

namespace Utility
{

class Directory
{
public:
   Directory(std::string path_);
   Directory(const Directory& other);

   class DirectoryIterator
   {
   public:
      DirectoryIterator();
      DirectoryIterator(std::string path);
      ~DirectoryIterator();

      bool operator!=(const Directory::DirectoryIterator& other);
      DirectoryEntry operator*();
      void operator++();

   private:
      bool open();
      bool close();

      DIR* dir;
      std::string path;
      dirent* last;
   };

   DirectoryIterator begin();
   DirectoryIterator end();
   
private:
   std::string path;
};

}

#endif

