#include <Debug.hh>

#include <FileSystem/FileSystem.hh>
#include <FileSystem/Stats.hh>

using namespace Vfs;

DoublyLinkedList<FileSystem> FileSystem::list;

FileSystem::FileSystem()
{
   list.insertLast(this);
}

FileSystem::~FileSystem()
{
   KASSERT(false);

//    for (auto vfs : list)
//    {
//       if (&vfs == this)
//       {
// //         vfs.remove();
//       }
//    }
}

FileSystem::ErrorCode FileSystem::mount(const char* path)
{
   ErrorCode error;

   for (auto& fs : list)
   {
      if ((error = fs.mount(path)) != ErrorCode::UnknownFsType)
      {
         return error;
      }
   }

   return ErrorCode::UnknownFsType;
}

FileSystem::ErrorCode FileSystem::umount()
{
   return ErrorCode::Ok;
}

FileSystem::ErrorCode FileSystem::stat(Stats& stat)
{
   stat.fileSystemId = 0;
   stat.blockSize = 512;
   stat.totalBlocks = 0;
   stat.freeBlocks = 0;
   stat.files = 0;

   return ErrorCode::Ok;
}

FileSystem::ErrorCode FileSystem::root(Node**)
{
   return ErrorCode::NoEntity;
}
