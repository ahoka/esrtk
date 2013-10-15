#include <FileSystem/Vfs.hh>
#include <FileSystem/VfsStats.hh>

DoublyLinkedList<Vfs> Vfs::list;

Vfs::Vfs()
{
   list.insertLast(this);
}

Vfs::~Vfs()
{
   for (auto vfs : list)
   {
      if (&vfs == this)
      {
//         vfs.remove();
      }
   }
}

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

