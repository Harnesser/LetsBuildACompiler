##
## Functions:
##  * read_int  - read an integer from stdin 
##  * atoi      - convert ascii text to an integer
.type read_int, @function
read_int:
# in bookeeping
pushl %ebp		# save old base pointer
movl %esp, %ebp		# make stack ptr = base ptr

# out bookkeeping
movl %ebp, %esp		# restore stack pointer
popl %ebp		# restore base pointer
ret




.type atoi, @function
atoi:
# in bookeeping
pushl %ebp		# save old base pointer
movl %esp, %ebp		# make stack ptr = base ptr

# out bookkeeping
movl %ebp, %esp		# restore stack pointer
popl %ebp		# restore base pointer
ret

