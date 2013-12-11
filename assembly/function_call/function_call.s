#
# Assembler function call example
#
# function computes x^y
# program computes 2^3 + 5^2
#
# From:
# http://programminggroundup.blogspot.com/2007/01/chapter-4-all-about-functions.html
#
# Look for a return value of 33 on the terminal.

.equ LINUX_SYSCALL, 0x80
.equ SYS_EXIT, 1

.section .data
.section .text

#
# MAIN
#
.globl _start
_start:

# Function call 1
pushl $3
pushl $2
call power
addl $8, %esp		# move stack pointer back (2*4)
pushl %eax 		# save answer before next call

# Function call 2
pushl $2
pushl $5
call power
addl $8, %esp

# Addition
popl %ebx		# first result
addl %eax, %ebx		# ebx = answer

# exit
#movl $3, %ebx		# return val
movl $SYS_EXIT, %eax
int $LINUX_SYSCALL

##
## Function: power
##
# Variables:
#  %ebx - holds base number
#  %ecx - holds the power
#  -4(%ebp) - holds current result
#  %eax used for temporary storage
.type power, @function
power:

# bookkeeping
pushl %ebp		# save old base pointer
movl %esp, %ebp		# make stack pointer = base pointer
subl $4, %esp		# local storage - 1 word

# grab arguments from stack
movl 8(%ebp), %ebx	# base number
movl 12(%ebp), %ecx	# power

movl %ebx, -4(%ebp)	# store current result

power_loop_start:
cmpl $1, %ecx		# we're done if the power is 1
je end_power

movl -4(%ebp), %eax	# move current result into accum
imull %ebx, %eax
movl %eax, -4(%ebp)
decl %ecx
jmp power_loop_start

end_power:
mov -4(%ebp), %eax	# return value goes in accum

# bookkeeping
movl %ebp, %esp		# restore stack pointer
popl %ebp		# restore base pointer
ret


