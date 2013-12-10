#
#
#
.equ LINUX_SYSCALL, 0x80
.equ SYS_EXIT, 1

.section .data

.section .text

#
# MAIN
#
.globl _start
_start:




# exit
movl $3, %ebx		# return val
movl $SYS_EXIT, %eax
int $LINUX_SYSCALL

