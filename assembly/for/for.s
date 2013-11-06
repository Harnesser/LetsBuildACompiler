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

.section .text
.globl _start
_start:

	pushl $10 # tgt
	pushl $0 # i
	movl  $0, %ebx  # accum i vals for test
.loop:
	popl %ecx
	popl %eax
	pushl %eax # push back immediately incase body clobbers it

	# test
	cmpl %eax, %ecx 
	jg .out

	# body
	addl %ecx, %ebx

	# incr
	addl $1, %ecx
	pushl %ecx

	# loop
	jmp .loop

.out:

movl $1, %eax      # this is the linux kernel command
                # number (system call) for exiting
                # a program

#movl $34, %ebx      # this is the status number we will
                # return to the operating system.
                # Change this around and it will
                # return different things to
                # echo $?

int $0x80          # this wakes up the kernel to run
                # the exit command
