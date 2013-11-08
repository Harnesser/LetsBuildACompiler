#PURPOSE:  Simple program that exits and returns a
#          status code back to the Linux kernel
#

#INPUT:    none
#

#OUTPUT:   returns a status code. This can be viewed
#          by typing
#
#          echo $?
#
#          after running the program
#

#VARIABLES:
#          %eax holds the system call number
#          %ebx holds the return status
#
.section .data
A:	.space 4
C:	.space 4
X:	.space 4
Y:	.space 4
Z:	.space 4
A0:	.space 4
COUNT:	.space 4
ANSWER:	.space 4
.section .text
.globl _start
_start:

.include "assembly.s"
# leaves the result in eax


movl %eax,%ebx
movl $1, %eax      # sys_exit
int $0x80           # call
