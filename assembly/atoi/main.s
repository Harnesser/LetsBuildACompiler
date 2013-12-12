##
## Test the read and atoi() functions
##

# linux error codes seem to be treated as +ve integers 0-255
# so numbers out of that range will not be printed properly
# until i write a itoa() subroutine,
#
# and this doesn't print negative numbers yet either...

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


.equ MBUFSIZE, 16
.equ CHAR_EQ, '='

.globl _start
_start:

subl $MBUFSIZE, %esp	# for ascii buffer

movl $0, %ecx
buf_clear_start:
	cmpl $MBUFSIZE, %ecx
	je buf_clear_done
	movb $CHAR_EQ, (%esp, %ecx)
	incl %ecx
	jmp buf_clear_start
buf_clear_done:
	

# call to read a number from stdin - no args
call read_int

# let's try to print it out again
pushl %esp
pushl $MBUFSIZE
pushl %eax
call itoa
#movl $MBUFSIZE, %eax
addl $12, %esp	# instead of popping...

# write
movl %eax, %edx
movl %esp, %ecx
movl $STDOUT, %ebx
movl $SYS_WRITE, %eax
int $LINUX_SYSCALL

# EXIT
epilogue:
movl %eax, %ebx
movl $SYS_EXIT, %eax
int $LINUX_SYSCALL
