#include "File.hh"

#include <errno.h>

using namespace Utility;

File::File(std::string path_)
   : path(path_),
     lastError(0)
{
}

File::File(File& other)
{
   if (this == &other)
   {
      return;
   }

   file = other.file;
   lastError = other.lastError;
   path = other.path;

   other.file = 0;
   other.lastError = 0;
}

File::~File()
{
   if (file != 0)
   {
      fclose(file);
   }
}

bool
File::open(const char* mode)
{
   file = fopen(path.c_str(), mode);

   if (file != 0)
   {
      lastError = errno;
      return false;
   }

   return true;
}

bool
File::close()
{
   int r = fclose(file);

   if (r == EOF)
   {
      lastError = errno;
   }

   // XXX on error too?
   file = 0;

   return r != EOF;
}

int
File::read(void *ptr, size_t size, size_t nmemb)
{
   int r = fread(ptr, size, nmemb, file);
   
   if (error())
   {
      lastError = errno;
   }

   return r;
}

int
File::write(const void *ptr, size_t size, size_t nmemb)
{
   int r = fwrite(ptr, size, nmemb, file);

   if (error())
   {
      lastError = errno;
   }

   return r;
}

bool
File::eof()
{
   return feof(file);
}

bool
File::error()
{
   return ferror(file);
}

bool
File::isOpen()
{
   return file != 0;
}
