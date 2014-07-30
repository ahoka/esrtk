#include <FileSystem/RamdiskNode.hh>

#include <sys/errno.h>

using namespace Vfs;

RamdiskNode::RamdiskNode()
{
}

RamdiskNode::~RamdiskNode()
{
}

Type
RamdiskNode::getType()
{
   return Type::File;
}

int
RamdiskNode::open(int)
{
   return EOPNOTSUPP;
}

int
RamdiskNode::close()
{
   return EOPNOTSUPP;
}

int
RamdiskNode::read(void *, size_t)
{
   return EOPNOTSUPP;
}

int
RamdiskNode::write(const void*, size_t)
{
   return EOPNOTSUPP;
}

int
RamdiskNode::createFile(Node**, const char*)
{
   return EOPNOTSUPP;
}

int
RamdiskNode::removeFile()
{
   return EOPNOTSUPP;
}

int
RamdiskNode::createDirectory(Node**, const char*)
{
   return EOPNOTSUPP;
}

int
RamdiskNode::removeDirectory()
{
   return EOPNOTSUPP;
}

int
RamdiskNode::createDevice(Node**, const char*)
{
   return EOPNOTSUPP;
}

int
RamdiskNode::removeDevice()
{
   return EOPNOTSUPP;
}

int
RamdiskNode::lookup(Node**, const char*)
{
   return EOPNOTSUPP;
}

int
RamdiskNode::sync()
{
   return EOPNOTSUPP;
}

int
RamdiskNode::control(uint32_t, void *)
{
   return EOPNOTSUPP;
}
