#include <FileSystem/Node.hh>

#include <sys/errno.h>

Node::Node()
{
}

Node::~Node()
{
}

int
Node::open(int)
{
   return EOPNOTSUPP;
}

int
Node::close()
{
   return EOPNOTSUPP;
}

int
Node::read(void *, size_t)
{
   return EOPNOTSUPP;
}

int
Node::write(const void*, size_t)
{
   return EOPNOTSUPP;
}

int
Node::remove()
{
   return EOPNOTSUPP;
}
