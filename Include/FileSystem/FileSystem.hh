#ifndef VFS_HH
#define VFS_HH

#include <DoublyLinkedItem.hh>
#include <DoublyLinkedList.hh>

#pragma GCC diagnostic ignored "-Wsuggest-final-types"

namespace Vfs
{
   struct Stats;
   class Node;

   class FileSystem : public DoublyLinkedItem<FileSystem>
   {
   public:
      FileSystem();
      virtual ~FileSystem();

      enum class ErrorCode
      {
         Ok,
         NoEntity,
         UnknownFsType
      };
      
      virtual ErrorCode mount(const char* path);
      virtual ErrorCode umount();
      virtual ErrorCode stat(Stats&);
      virtual ErrorCode root(Node**);

   private:
      FileSystem(const FileSystem&) = delete;
      FileSystem& operator=(const FileSystem&) = delete;

      static DoublyLinkedList<FileSystem> list;
   };

};

#endif
