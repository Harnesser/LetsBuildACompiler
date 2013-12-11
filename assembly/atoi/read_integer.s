##
## Functions:
##  * read_int  - read an integer from stdin 
##  * atoi      - convert ascii text to an integer

.equ BUFSIZE, 16	# multiple of 4 for alignment?

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

call atoi
addl $5, %eax		# result in accum

# out bookkeeping
movl %ebp, %esp		# restore stack pointer
popl %ebp		# restore base pointer
ret




.type atoi, @function
atoi:
# in bookeeping
pushl %ebp		# save old base pointer
movl %esp, %ebp		# make stack ptr = base ptr

movl $5, %eax		# result in accum
# out bookkeeping
movl %ebp, %esp		# restore stack pointer
popl %ebp		# restore base pointer
ret

