#ifndef __FILE_HH__
#define __FILE_HH__

#include <cstdio>
#include <string>

namespace Utility
{

class File
{
public:
   File(std::string path);
   File(File& other); // loses ownership on copy!
   ~File();

   bool open(const char* mode = "r");
   bool close();

   int read(void *ptr, size_t size, size_t nmemb);
   int write(const void *ptr, size_t size, size_t nmemb);

   bool seek(long offset);
   void rewind();

   bool eof();
   bool error();
   int getLastError();
   bool isOpen();

   std::string getPath();

   enum Mode
   {
      Read,
      Write,
      Create,
      Append
   };

private:
   std::string path;
   FILE* file;
   int lastError;
};

}

#endif
