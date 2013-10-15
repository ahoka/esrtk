#ifndef FILESYSTEMID_HH
#define FILESYSTEMID_HH

#include <cstdint>


// XXX namespace?
typedef uint32_t FileSystemId;

enum FsId
{
   NullFileSystem,
   RamdiskFs
};

#endif
