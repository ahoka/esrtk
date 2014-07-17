#ifndef NODE_HH
#define NODE_HH

// represent a file node (vnode)

#include <cstddef>

class Node
{
public:
   Node();
   virtual ~Node();

   virtual int open(int flags);
   virtual int close();
   virtual int read(void *, size_t);
   virtual int write(const void*, size_t);
   virtual int remove();

private:
   Node& operator=(const Node&) = delete;
   Node(const Node&) = delete;
};

#endif
