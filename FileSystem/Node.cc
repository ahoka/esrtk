#include <FileSystem/Node.hh>

#include <sys/errno.h>

using namespace Vfs;

Node::Node()
{
}

Node::~Node()
{
}

Type
Node::getType()
{
   return Type::File;
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
Node::createFile(Node**, const char*)
{
   return EOPNOTSUPP;
}

int
Node::removeFile()
{
   return EOPNOTSUPP;
}

int
Node::createDirectory(Node**, const char*)
{
   return EOPNOTSUPP;
}

int
Node::removeDirectory()
{
   return EOPNOTSUPP;
}

int
Node::createDevice(Node**, const char*)
{
   return EOPNOTSUPP;
}

int
Node::removeDevice()
{
   return EOPNOTSUPP;
}

int
Node::lookup(Node**, const char*)
{
   return EOPNOTSUPP;
}

int
Node::sync()
{
   return EOPNOTSUPP;
}

int
Node::control(uint32_t, void *)
{
   return EOPNOTSUPP;
}
