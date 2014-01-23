# Implementing Function Calls 
 I'm going to diverge from the text here a bit. I'm going to keep modifying
the main program, instead of trying it on with the vestigal single char
program he's got going.

## Linux x86 Function Calls
 From messing around with the READ and WRITE function underlying x86 assembly
code, I've already encountered how function calls are implemented in asm. I'm
going to repeat what I've learnt here, and use this as a design document of
sorts.

### Entry and Exit
 Function calls, at their heart, are program counter modifications. When a
function is called, the program counter is set to the address of where the 
code for that function is located. When we're done in the function, we pop
back to just after where we came from and keep going from there. This looks
something like the code below - `call` and `ret` do the program counter
tweaking:

    .globl _start
    _start:
    		<ASM>
     		call my_func
    		<ASM>
    		int SYS_EXIT
    
    .type atoi, @function
    my_func:
    		<ASM>
    		ret

 In the code above, the `.globl _start` tells linux where to start the program,
and the `.type` line is a thing for the `as` tool to help mark functions.

### Options for Data
 The code above is awesome if you don't want to pass any data to your function, 
or return data from it for that matter.

 Let's start with getting data into our function call. We've a few options 
including using the registers or using the stack. We'll go with using the stack
for input data, and we'll return data in the accumulator (`%eax`).

### Stack Recap
 The stack is a place to store data. Data is `push`ed and `pop`ed from the stack,
and the Extended Stack Pointer (`%esp`) always points to the 'top' of the stack.
But since in x86 the stack grows downwards in memory addresses, pushing data on
to the stack *decreases* `%esp`!

 When function calls are involved, another pointer called the Extended Base 
Pointer (`%ebp`) keeps track of where the base of the stack was in the calling
function. These portions of the overall stack that are particular to each
function call are called 'stack frames'.

 Another thing to note, is that I'll be using the 'l' version of instructions - 
these operate on words that are 4 bytes long.

 When pushing data on to the stack with `pushl` the stack grows and `%esp` 
*decreases* by 4 (yes, decreases - remember the stack grows downwards in memory
addresses. The 4 is the word length in bytes.

 Popping from the stack makes it shrink - `%esp` *increases* by 4 when `popl`
is executed.

### Passing in Data
 To pass data to a function call, we'll just pop the data on to the stack. So
if we need to pass 3 words to the stack, we'll `pushl` three times. The stack
frame we have before a call will look a little like this (I've started from an
arbitrary address):

    200 <data>   
    196 <data>
    192 <data>   <- %esp

 To pass data to a function call, we'll just pop the data on to the stack. So
if we need to pass 2 words to the stack, we'll `pushl` two times. Now the stack
frame will look like:

    200 <data>   
    196 <data>
    192 <data>
    188 <argument 2>
    184 <argument 1>      <- %esp

 In x86, the `call` instruction pushes the return address onto the stack before 
calling the function. When we enter the function, the stack looks like below:

    200 <data>   
    196 <data>
    192 <data>
    188 <argument 2>
    184 <argument 1>
    180 <return address>   <- %esp

 The first thing we'll need to do is store the pointer to the calling stack
frame so that it can be restored when the function returns. So we push `%ebp`
on to the stack - `%ebp` is the Base Pointer and by convention it is used
uniquely for keeping track of stack frames. Stack now looks like this:

    200 <data>   
    196 <data>
    192 <data>
    188 <argument 2>
    184 <argument 1>
    180 <return address>
        -------------------------------------------- Stack frame 
    176 <old base pointer> <- %esp

### Reserving Space for Local Variables
 We'll now play with `%ebp` and `%esp` to make accessing function input data 
easier and to reserve space for variables local to the function call. We'll
copy the stack pointer to the base pointer...

    200 <data>   
    196 <data>
    192 <data>
    188 <argument 2>
    184 <argument 1>
    180 <return address>
        -------------------------------------------- Stack frame 
    176 <old base pointer> <- %esp  <- %ebp

... and subtract from the stack pointer to reserve space on the stack for
local variables. Say we need 3 local variables, so we'll take 3*4 = 12
from `%esp` (remember: words = 4 bytes, stack grows downwards).

    200 <data>
    196 <data>
    192 <data>
    188 <argument 2>
    184 <argument 1>
    180 <return address>
        -------------------------------------------- Stack frame 
    176 <old base pointer>          <- %ebp
    172 <local var 1>
    168 <local var 2>
    164 <local var 3>               <- %esp

 The beauty of this setup is that we can now access input arguments and
local variables using the base pointer `%ebp` as a reference. Arguments
are `%ebp` plus something, and local variables are `%ebp` minus something.
For example:

* first argument is at `8(%ebp)`
* second argument at `12(%ebp)`
* first local variable at `-4(%ebp)`
* second local variable at `-8(%ebp)`

### Returning Data
 For this exercise, I'm just going to have functions return one word of
data (which could be an address!) in the accumulator (`%eax`). That's that
done.

### Cleanup
 After all that vandalism on `%ebp` and `%esp`, we'll put things right before
we return from the call. This means:

(1). Restoring the stack pointer by setting it to `%ebp`:

    200 <data>
    196 <data>
    192 <data>
    188 <argument 2>
    184 <argument 1>
    180 <return address>
        -------------------------------------------- Stack frame
    176 <old base pointer>          <- %ebp   <- %esp
    172 <local var 1>
    168 <local var 2>
    164 <local var 3>

(2). restoring the old base pointer by `pop`ing into `%ebp`:

    ???                                       <- %ebp
        ...
    200 <data>
    196 <data>
    192 <data>
    188 <argument 2>
    184 <argument 1>
    180 <return address>                      <- %esp
        -------------------------------------------- Stack frame
    176 <old base pointer>
    172 <local var 1>
    168 <local var 2>
    164 <local var 3>      

 Now we can call `ret` to, well, return from the function feeling good that
we've been a good citizen.

## Function Call Implementation in KISS
 Given that I'm diverging from the text a little, I should explain how I've
implemented the function calls. 

Highlights:

* no nested procedures
* local variables
* pass by ???

### Declaring a Function

BNF:

    <declaration> ::= <data-decl> | <procedure> | <main-program>
    <procedure> ::= PROCEDURE <ident> <begin-block>
    <main-program> ::= PROGRAM <ident> <begin-block>

Example program:

    PROCEDURE func;
    VAR a = 2;
    BEGIN
        a = a + 2;
    END

    PROGRAM main;
    VAR a = 0;
    VAR b = 0;
    BEGIN
        c = a + func(a);
    END
    
### Calling a Function


### Passing Parameters
* pass by value/reference
* function calls as arguments

### Local Variables
* symbol table

### Assembly

