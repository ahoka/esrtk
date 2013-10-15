#include <FileSystem/Vfs.hh>
#include <FileSystem/VfsStats.hh>

Vfs::ErrorCode Vfs::mount(const char* /*path*/)
{
   return ErrorCode::Ok;
}

Vfs::ErrorCode Vfs::umount()
{
   return ErrorCode::Ok;
}

Vfs::ErrorCode Vfs::stat(VfsStats& stat)
{
   stat.fileSystemId = 0;
   stat.blockSize = 512;
   stat.totalBlocks = 0;
   stat.freeBlocks = 0;
   stat.files = 0;

   return ErrorCode::Ok;
}

