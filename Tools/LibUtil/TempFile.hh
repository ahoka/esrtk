#ifndef __TEMPFILE_HH__
#define __TEMPFILE_HH__

#include "File.hh"

#include <string>

namespace Utility
{

class TempFile : public File
{
public:
   TempFile(std::string filename);
private:
   static std::string createTempFile(std::string filename);
};

}

#endif
