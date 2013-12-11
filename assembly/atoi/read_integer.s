##
## Functions:
##  * read_int  - read an integer from stdin 
##  * atoi      - convert ascii text to an integer

.equ BUFSIZE, 16	# multiple of 4 for alignment?

# ----------------------------------------------------
# read_int - uses the SYS_READ call to read in some
# ascii chars from stdin. Then calls atoi() to convert
# this to an integer which ops can be performed on.
# number is returned in the accumulator
.type read_int, @function
read_int:

# in bookeeping
pushl %ebp		# save old base pointer
movl %esp, %ebp		# make stack ptr = base ptr
subl $BUFSIZE, %esp

# call to sys_read
# stack grows by decreasing addresses.
# looks like sys_read writes to increasing addresses.
# so pass it the bottom of the stack, which is %esp
movl $BUFSIZE, %edx
movl %esp, %ecx
movl $STDIN, %ebx
movl $SYS_READ, %eax
int $LINUX_SYSCALL
# read size in accum

# Call ascii_to_integer (atoi)
pushl %esp
pushl %eax
call atoi

# out bookkeeping
movl %ebp, %esp		# restore stack pointer
popl %ebp		# restore base pointer
ret

.equ CHAR_0, '0'
.equ CHAR_9, '9'
.equ CHAR_MINUS, '-'

# ----------------------------------------------------
# TODO: needs a companion itoa()
#
# Register usage:
#  %eax - integer built up in accum
#  %ebx - int value of current ascii char
#  %ecx - character countdown
#  %edx - current ascii char addr on stack
#
# Local variables:
#  -4(%ebp) - 1 if first char is '-'. Used to negate
#             final accum variable.
#
.type atoi, @function
atoi:
# in bookeeping
pushl %ebp		# save old base pointer
movl %esp, %ebp		# make stack ptr = base ptr
subl $4, %esp

movl 8(%ebp), %ecx	# char count 
movl 12(%ebp), %edx	# ascii start addr

# check if we're negative
movb (%edx,1), %bl
movl $0, -4(%ebp)
cmpb $CHAR_MINUS, %bl
jne atoi_init
movl $1, -4(%ebp)
eat_neg_sign:
incl %edx
decl %ecx

atoi_init:
movl $0, %eax

atoi_loop_start:
jecxz atoi_loop_done

movb (%edx,1), %bl	# load next char into b

# bounds check - needs to be between '0' and '9'
bounds_check:
cmpb $CHAR_0, %bl
jl next_char
cmpb $CHAR_9, %bl
jg next_char

# convert - ascii code - '0' gives int value of char
subb $CHAR_0, %bl
imull $10, %eax
addl %ebx, %eax

# get next char - update char pointer and counter
next_char:
incl %edx		# increment char addr
decl %ecx		# decrement char counter
jmp atoi_loop_start

atoi_loop_done:
# are we negative?
cmpl $1, -4(%ebp)
jne atoi_epilogue
negl %eax

# out bookkeeping
atoi_epilogue:
movl %ebp, %esp		# restore stack pointer
popl %ebp		# restore base pointer
ret

