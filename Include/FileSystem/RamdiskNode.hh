#include <FileSystem/Node.hh>

#include <FileSystem/Ramdisk.hh>

namespace Vfs
{
   class RamdiskNode
   {
   public:
      RamdiskNode();
      virtual ~RamdiskNode();

      virtual Type getType();

      // File
      virtual int open(int flags);
      virtual int close();
      virtual int read(void *, size_t);
      virtual int write(const void*, size_t);
      virtual int sync();

      // Directory
      virtual int createFile(Node**, const char*);
      virtual int removeFile();
      virtual int createDirectory(Node**, const char*);
      virtual int removeDirectory();
      virtual int createDevice(Node**, const char*);
      virtual int removeDevice();
      virtual int lookup(Node**, const char*);

      // Device
      virtual int control(uint32_t, void *);

   private:
      RamdiskNode& operator=(const RamdiskNode&) = delete;
      RamdiskNode(const RamdiskNode&) = delete;

      Ramdisk* ramdiskM;
   };
};
