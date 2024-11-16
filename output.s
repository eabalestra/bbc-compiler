.file "output.s"
.text

inc:
    enter   $(8 * 2), $0 
    movl   %rdi, -8(%rbp)

    movl   -8(%rbp), %r10
    addl   $1, %r10
    movl   %r10, -16(%rbp)

    mov   -16(%rbp), %rax 
    leave 
    ret

get_integer:
    enter   $(8 * 0), $0 


printInteger:
    enter   $(8 * 1), $0 
    movl   %rdi, -8(%rbp)

    .globl main

main:
    enter   $(8 * 6), $0 

    call   get_integer
    movl   %eax, -16(%rbp)

    movl   $0, %r10
    movl   %r10, -16(%rbp)

    call   inc
    movl   %eax, -24(%rbp)

    movl   $1, %r10
    movl   %r10, -24(%rbp)

    movl   -24(%rbp), %r10
    movl   %r10, -0(%rbp)

    movl   -8(%rbp), %r10
    cmpl   $1, %r10
    sete   %al
    movzbl %al, %eax
    movl   %eax, -32(%rbp)

    call   printInteger
    movl   %eax, -40(%rbp)

    movl   $1, %r10
    movl   %r10, -40(%rbp)

    call   printInteger
    movl   %eax, -48(%rbp)

    movl   $1, %r10
    movl   %r10, -48(%rbp)

    mov   $0, %rax 
    leave 
    ret
.ident   "GCC: (Ubuntu 13.2.0-23ubuntu4) 13.2.0" 
