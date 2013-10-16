#ifndef VFS_HH
#define VFS_HH

#include <DoublyLinkedItem.hh>
#include <DoublyLinkedList.hh>

struct VfsStats;

class Vfs : public DoublyLinkedItem<Vfs>
{
public:
   Vfs();
   virtual ~Vfs();

   enum class ErrorCode
   {
      Ok,
      UnknownFsType
   };

   ErrorCode mount(const char* path);
   ErrorCode umount();
   ErrorCode stat(VfsStats&);

private:
   Vfs(const Vfs&) = delete;

   static DoublyLinkedList<Vfs> list;
};

#endif
