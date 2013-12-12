##
## Functions:
##  * read_int  - read an integer from stdin 
##  * atoi      - convert ascii text to an integer
##  * itoa      - convert a number into base10 string
##

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


# ----------------------------------------------------
#
# Arguments:
#   8(%ebp) - number to convert
#  12(%ebp) - buffer size
#  16(%ebp) - buffer address
#
# Register usage:
#  %eax - integer built up in accum
#  %ebx - int value of current ascii char
#  %ecx - character countdown
#  %edx - current ascii char addr on stack
#
# Local variables:
#  -4(%ebp) - actual char count of ascii version
#  -8(%ebp) - 1 if first char is '-'. Used to negate
#             final accum variable.
#
.type itoa, @function
itoa:

itoa_prologue:
	pushl %ebp
	movl %esp,%ebp

	# space for local variables
	subl $4, %esp		# -4: char count of final string
	subl $4, %esp		# -8: if 1, add a - sign cos number is negative
	subl 12(%ebp), %esp	# buffer starts at %esp now

	# Grab arguments
	movl 8(%ebp), %eax	# number to convert

check_if_negative:
	movl $0, -8(%ebp)	# default - positive
	cmpl $0, %eax
	jge itoa_loop_init
	negl %eax
	movl $1, -8(%ebp)

itoa_loop_init:
	movl $0, %ecx

itoa_loop_start:
	movl $0, %edx

itoa_check:
	cmpl 12(%ebp), %ecx	# check that buffer has space
	je itoa_loop_done

itoa_check_last_digit:
	cmpl $10, %eax		# if num < 10, just convert it
	jl itoa_last_digit

itoa_dir:
	movl $10, %ebx
	divl %ebx		# accum = quotient, edx = remainder

# convert the remainder to ascii and add it to the buffer
itoa_convert:
	addl $CHAR_0, %edx
	movb %dl, (%esp, %ecx, 1)

	incl %ecx
	jmp itoa_loop_start

itoa_last_digit:
	# should be left with a number 0 to 9, so convert it
	# we have space
	addl $CHAR_0, %eax
	movb %al, (%esp, %ecx, 1)

itoa_loop_done:
	movl %ecx, -4(%ebp)		# store total char count
	incl -4(%ebp)

# reverse-copy the local buffer to the results buffer
movl 16(%ebp), %ebx

itoa_maybe_negate:
	# was the number negative?
	cmpl $1, -8(%ebp)
	jne itoa_reverse_copy

	# do we have space for the '-'?
	# if not, return a length of 0
	cmpl 12(%ebp), %eax
	jl itoa_no_space_for_neg
	movl $CHAR_MINUS, (%ebx)
	incl -4(%ebp)		# incr string length
	incl %ebx		# next char ptr update
	jmp itoa_reverse_copy

itoa_reverse_copy:
	cmpl $-1, %ecx
	je itoa_reverse_copy_done
	movb (%esp, %ecx, 1), %al
	movb %al, (%ebx)
	decl %ecx
	incl %ebx
	jmp itoa_reverse_copy

itoa_reverse_copy_done:

itoa_no_space_for_neg:
	mov $0, %eax 		# flag error with 0 length string

itoa_epilogue:
	movl -4(%ebp), %eax	# return char count in accum
	movl %ebp, %esp		# restore stack pointer
	popl %ebp		# restore base pointer
	ret

