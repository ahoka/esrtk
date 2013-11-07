#include "DirectoryEntry.hh"

using namespace Utility;

DirectoryEntry::DirectoryEntry(struct dirent de, std::string parent)
   : dent(de),
     directory(parent)
{
}

std::string
DirectoryEntry::getPath()
{
   return directory + "/" + dent.d_name;
}

std::string
DirectoryEntry::getName()
{
   return std::string(dent.d_name);
}

bool
DirectoryEntry::isFile()
{
   return dent.d_type != DT_DIR;
}

bool
DirectoryEntry::isDirectory()
{
   return dent.d_type == DT_DIR;
}
