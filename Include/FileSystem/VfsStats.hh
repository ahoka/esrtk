#ifndef VFSSTATS_HH
#define VFSSTATS_HH

#include <FileSystem/FileSystemId.hh>
#include <cstdint>

struct VfsStats
{
   FileSystemId fileSystemId;
   uint32_t blockSize;
   uint32_t totalBlocks;
   uint32_t freeBlocks;
   uint32_t files;
};

#endif
