.global _start
.global ___putc
.global _mmap
.global setjmp
.global longjmp
.type _start,@function
.type ___putc,@function
.type _mmap,@function
.type setjmp,@function
.type longjmp,@function

_start:
   mov %rsp, %rbp
   mov 0(%rbp), %rdi
   lea 8(%rbp), %rsi
   call _wrap_main
   mov %rax, %rdi
   mov $60, %rax
   syscall

___putc:
   dec %rsp
   mov %rbx, %r8
   mov %rdi, %rax
   mov %al, 0(%rsp)
   mov $1, %edi
   mov %rsp, %rsi
   mov $1, %edx
   mov $1, %eax
   syscall
   inc %rsp
   mov %r8, %rbx
   ret
  
_mmap:
   mov $9, %eax
   mov $0, %rdi
   mov $0x6400000, %rsi # 100Mb
   mov $3, %rdx
   mov $0x22, %r10
   mov $-1, %r8
   mov $0, %r9
   syscall
   ret 

setjmp:
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

longjmp:
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
