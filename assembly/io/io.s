#
#
#
.equ LINUX_SYSCALL, 0x80

.equ SYS_EXIT, 1
.equ SYS_READ, 3
.equ SYS_WRITE, 4

.equ STDIN, 0
.equ STDOUT, 1
.equ STDERR, 2

.section .data

.section .bss
.equ BUFSIZE, 32
.lcomm buffer, BUFSIZE

.section .text

#
# MAIN
#
.globl _start
_start:

# read
movl $BUFSIZE, %edx
movl $buffer, %ecx
movl $STDIN, %ebx
movl $SYS_READ, %eax
int $LINUX_SYSCALL

# write
movl $BUFSIZE, %edx
movl $buffer, %ecx
movl $STDOUT, %ebx
movl $SYS_WRITE, %eax
int $LINUX_SYSCALL

# exit
movl $3, %ebx		# return val
movl $SYS_EXIT, %eax
int $LINUX_SYSCALL

