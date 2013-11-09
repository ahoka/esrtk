#include "Directory.hh"

#include <errno.h>
#include <cstdlib>

using namespace Utility;

Directory::Directory(std::string path_)
{
   char buf[PATH_MAX];

   path = realpath(path_.data(), buf);
}

Directory::Directory(const Directory& other)
   : path(other.path)
{
}

Directory::DirectoryIterator
Directory::begin()
{
   return DirectoryIterator(path);
}

Directory::DirectoryIterator
Directory::end()
{
   return DirectoryIterator();
}

Directory::DirectoryIterator::DirectoryIterator()
   : last(0)
{
}

Directory::DirectoryIterator::DirectoryIterator(std::string path_)
   : dir(0),
     path(path_),
     last(0)
{
   if (open())
   {
      last = readdir(dir);
   }
}

Directory::DirectoryIterator::~DirectoryIterator()
{
   close();
}

bool
Directory::DirectoryIterator::operator!=(const Directory::DirectoryIterator& other)
{
   return last != other.last;
}

DirectoryEntry
Directory::DirectoryIterator::operator*()
{
   return DirectoryEntry(*last, path);
}

void
Directory::DirectoryIterator::operator++()
{
   if (last != 0)
   {
      last = readdir(dir);
   }
}

bool
Directory::DirectoryIterator::open()
{
   dir = opendir(path.data());
   if (dir == 0)
   {
      return false;
   }

   return true;
}

bool
Directory::DirectoryIterator::close()
{
   if (dir == 0)
   {
      return false;
   }

   if (closedir(dir) == -1)
   {
      return false;
   }

   return true;
}
