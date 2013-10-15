#ifndef VNODEOPERATIONS_HH
#define VNODEOPERATIONS_HH

class VnodeOperations
{
   virtual int open(const char* name, Vnode* vnode) = 0;
   virtual int close() = 0;
   virtual int read() = 0;
   virtual int write() = 0;
   virtual int create() = 0;
   virtual int remove() = 0;
};

#endif
