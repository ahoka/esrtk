#include "File.hh"

#include <errno.h>

using namespace Utility;

File::File(std::string path_)
   : path(path_),
     file(0),
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
      std::fclose(file);
   }
}

std::string
File::getPath()
{
   return path;
}

bool
File::open(const char* mode)
{
   file = std::fopen(path.c_str(), mode);

   if (file == 0)
   {
      lastError = errno;
      return false;
   }

   return true;
}

bool
File::close()
{
   int r = std::fclose(file);

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
   int r = std::fread(ptr, size, nmemb, file);
   
   if (error())
   {
      lastError = errno;
   }

   return r;
}

int
File::write(const void *ptr, size_t size, size_t nmemb)
{
   int r = std::fwrite(ptr, size, nmemb, file);

   if (error())
   {
      lastError = errno;
   }

   return r;
}

bool
File::seek(long offset)
{
   int r = std::fseek(file, offset, SEEK_SET);
   if (r == -1)
   {
      lastError = errno;

      return false;
   }

   return true;
}

void
File::rewind()
{
   std::rewind(file);
}

bool
File::eof()
{
   return std::feof(file);
}

bool
File::error()
{
   return std::ferror(file);
}

bool
File::isOpen()
{
   return file != 0;
}

int
File::getLastError()
{
   return lastError;
}
