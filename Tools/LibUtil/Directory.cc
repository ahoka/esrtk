#include "Directory.hh"

#include <errno.h>

using namespace Utility;

Directory::Directory(std::string path_)
{
   char buf[PATH_MAX];

   path = realpath(path_.c_str(), buf);
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
   : path(path_),
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
   dir = opendir(path.c_str());
   if (dir == 0)
   {
      return false;
   }

   return true;
}

bool
Directory::DirectoryIterator::close()
{
   if (closedir(dir) == -1)
   {
      return false;
   }

   return true;
}
