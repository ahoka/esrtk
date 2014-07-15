#ifndef RAMDISKFS_HH
#define RAMDISKFS_HH

#include <FileSystem/FileSystem.hh>
#include <FileSystem/Stats.hh>

class RamdiskFs : public Vfs::FileSystem
{
   RamdiskFs();

   ErrorCode mount(const char* path);
   ErrorCode umount(const char* path);
   ErrorCode stat(const char* path, Vfs::Stats&);
};

#endif
