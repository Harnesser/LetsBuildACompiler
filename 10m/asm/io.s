##
## Functions:
##  * read_int  - read an integer from stdin 

.include "linux.s"

.equ BUFSIZE, 16	# multiple of 4 for alignment?
.equ BBUFSIZE, 20
.equ CHAR_NEWLINE, 0x0A

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


# ----------------------------------------------------
# write_int - uses the SYS_WRITE call to print out the
# value stored in the accum. Calls itoa() to build an
# ascii repr of the number
.type write_int, @function
write_int:

# in bookeeping
pushl %ebp		# save old base pointer
movl %esp, %ebp		# make stack ptr = base ptr
subl $BBUFSIZE, %esp	# string buffer - 4 chars larger
                        #  than what we're telling the other
                    	#  functions

# first, get ascii rep of %eax
pushl %esp
pushl $BUFSIZE
pushl %eax
call itoa
addl $12, %esp		# instead of popping 3 times...

# add newline
movl $CHAR_NEWLINE, (%esp, %eax)
incl %eax

# write
movl %eax, %edx
movl %esp, %ecx
movl $STDOUT, %ebx
movl $SYS_WRITE, %eax
int $LINUX_SYSCALL

# out bookkeeping
movl %ebp, %esp		# restore stack pointer
popl %ebp		# restore base pointer
ret

