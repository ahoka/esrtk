#ifndef RAMDISKFS_HH
#define RAMDISKFS_HH

#include <FileSystem/Vfs.hh>

class RamdiskFs : public Vfs
{
   RamdiskFs();

   ErrorCode mount(const char* path);
   ErrorCode umount(const char* path);
   ErrorCode stat(const char* path, VfsStats&);
};

#endif
