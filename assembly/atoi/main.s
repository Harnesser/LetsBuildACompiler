##
## Test the read and atoi() functions
##

.equ LINUX_SYSCALL, 0x80

.equ SYS_EXIT, 1
.equ SYS_READ, 3
.equ SYS_WRITE, 4

.equ STDIN, 0
.equ STDOUT, 1
.equ STDERR, 2

#.section .data
#.section .bss

.section .text
.include "read_integer.s"

.globl _start
_start:


# EXIT
movl $3, %ebx
movl $SYS_EXIT, %eax
int $LINUX_SYSCALL
