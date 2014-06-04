#include <Assembly.hh>

FUNCTION(spinlock_enter)
	movl $0, %eax
	movl $1, %ecx
	movl 4(%esp), %edx
	lock cmpxchg %ecx, (%edx)
	jz 1f
	pause
	jmp spinlock_enter
1:	
	ret
END(spinlock_enter)

FUNCTION(spinlock_try)
	movl $0, %eax
	movl $1, %ecx
	movl 4(%esp), %edx
	lock cmpxchg %ecx, (%edx)
	ret
END(spinlock_try)

FUNCTION(spinlock_exit)
	movl 4(%esp), %edx
	movl (%edx), %eax
	movl $0, (%edx)
	ret
END(spinlock_exit)
