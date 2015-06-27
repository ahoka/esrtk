#include "Assembly.hh"

FUNCTION(do_syscall)
        movl 4(%esp), %eax
        movl 8(%esp), %ecx
        int $0x80
        ret
END(do_syscall)
