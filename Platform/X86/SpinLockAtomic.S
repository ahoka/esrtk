.global spinlock_lock
spinlock_lock:
	movl $0, %eax
	movl $1, %ecx
	movl 4(%esp), %ebx
	lock cmpxchg %ecx, (%ebx)
	jz 1f
	pause
	jmp spinlock_lock
1:	
	ret

.global spinlock_try
spinlock_try:
	movl $0, %eax
	movl $1, %ecx
	movl 4(%esp), %ebx
	lock cmpxchg %ecx, (%ebx)
	ret

.global spinlock_unlock
spinlock_unlock:
	//movl $1, %eax
	//movl $0, %ecx
	movl 4(%esp), %ebx
	//lock cmpxchg %ecx, (%ebx)
	movl (%ebx), %eax
	movl $0, (%ebx)
	ret
