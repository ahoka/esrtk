#include <FileSystem/RamdiskFs.hh>
#include <FileSystem/VfsStats.hh>

RamdiskFs::RamdiskFs()
{
}

Vfs::ErrorCode
RamdiskFs::mount(const char* /*path*/)
{
   return ErrorCode::Ok;
}

Vfs::ErrorCode
RamdiskFs::umount(const char* /*path*/)
{
   return ErrorCode::Ok;
}

Vfs::ErrorCode
RamdiskFs::stat(const char* /*path*/, VfsStats& stat)
{
   stat.fileSystemId = 0;
   stat.blockSize = 512;
   stat.totalBlocks = 0;
   stat.freeBlocks = 0;
   stat.files = 0;

   return ErrorCode::Ok;
}
