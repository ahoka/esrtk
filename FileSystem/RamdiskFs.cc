#include <FileSystem/RamdiskFs.hh>
#include <FileSystem/Stats.hh>

using namespace Vfs;

RamdiskFs::RamdiskFs()
{
}

FileSystem::ErrorCode
RamdiskFs::mount(const char* /*path*/)
{
   return ErrorCode::Ok;
}

FileSystem::ErrorCode
RamdiskFs::umount(const char* /*path*/)
{
   return ErrorCode::Ok;
}

FileSystem::ErrorCode
RamdiskFs::stat(const char* /*path*/, Stats& stat)
{
   stat.fileSystemId = 0;
   stat.blockSize = 4096;
   stat.totalBlocks = 0;
   stat.freeBlocks = 0;
   stat.files = 0;

   return ErrorCode::Ok;
}
