.global start
.global ____putc
.global __mmap
.global _setjmp
.global _longjmp

start:
   mov %rsp, %rbp
   mov 0(%rbp), %rdi
   lea 8(%rbp), %rsi
   call __wrap_main
   mov %rax, %rdi
   mov $0x2000001, %rax
   syscall

____putc:
   dec %rsp
   mov %rbx, %r8
   mov %rdi, %rax
   mov %al, 0(%rsp)
   mov $1, %edi    # using stdout
   mov %rsp, %rsi  # point to the buffer
   mov $1, %edx    # buffer is only 1 char
   mov $0x2000004, %eax    # write syscall 0x4
   syscall
   inc %rsp
   mov %r8, %rbx
   ret
  
__mmap:
   mov $0x20000C5, %eax # mmap syscall 0xC5 or 197
   mov $0, %rdi          # don't map
   mov $0x640000000, %rsi  # size 100Mb 
   mov $3, %rdx         
   mov $0x1002, %r10
   mov $-1, %r8
   mov $0, %r9
   syscall
   ret 

_setjmp:
	mov %rbx, 0(%rdi)
	mov %rbp, 8(%rdi)
	mov %r12, 16(%rdi)
	mov %r13, 24(%rdi)
	mov %r14, 32(%rdi)
	mov %r15, 40(%rdi)
	lea 8(%rsp), %rdx
	mov %rdx, 48(%rdi)
	mov (%rsp), %rdx
	mov %rdx, 56(%rdi)
	xor %rax, %rax
	ret

_longjmp:
	mov %rsi,  %rax
	test %rax, %rax
	jnz 1f
	inc %rax
1:
	mov 0(%rdi), %rbx
	mov 8(%rdi), %rbp
	mov 16(%rdi), %r12
	mov 24(%rdi), %r13
	mov 32(%rdi), %r14
	mov 40(%rdi), %r15
	mov 48(%rdi), %rdx
	mov %rdx, %rsp
	mov 56(%rdi), %rdx
	jmp *%rdx
