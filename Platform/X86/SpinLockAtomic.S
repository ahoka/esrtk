.global spinlock_lock
spinlock_lock:
	movl $0, %eax
	movl $1, %ecx
	lock cmpxchg %ecx, 4(%esp)
	jz 1f
	pause
	jmp spinlock_lock
1:	
	ret

.global spinlock_unlock
spinlock_unlock:
	movl $1, %eax
	movl $0, %ecx
	lock cmpxchg %ecx, 4(%esp)
	ret
