#include "Assembly.hh"

FUNCTION(syscall)
        movl 4(%esp), %eax
        movl 8(%esp), %ecx
        int $0x80
        ret
END(syscall)
