#ifndef VFS_HH
#define VFS_HH

struct VfsStats;

class Vfs
{
public:
   Vfs();
   virtual ~Vfs();

   enum class ErrorCode
   {
      Ok,
      Error
   };

   ErrorCode mount(const char* path);
   ErrorCode umount();
   ErrorCode stat(VfsStats&);
};

#endif
