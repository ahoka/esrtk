#ifndef RAMDISKFS_HH
#define RAMDISKFS_HH

#include <FileSystem/FileSystem.hh>
#include <FileSystem/Stats.hh>

#include <FileSystem/Ramdisk.hh>

class RamdiskFs : public Vfs::FileSystem
{
public:
   RamdiskFs();

   ErrorCode mount(const char* path);
   ErrorCode umount(const char* path);
   ErrorCode stat(const char* path, Vfs::Stats&);
   ErrorCode root(Vfs::Node**);
private:
   
   Ramdisk* ramdiskM;
};

#endif
