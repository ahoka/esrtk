#ifndef NODE_HH
#define NODE_HH

// represent a file node (vnode)

#include <cstddef>
#include <cstdint>

namespace Vfs
{
   enum class Type
   {
      File,
      Directory
   };

   class Node
   {
   public:
      Node();
      virtual ~Node();

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
      Node& operator=(const Node&) = delete;
      Node(const Node&) = delete;
   };
};

#endif
