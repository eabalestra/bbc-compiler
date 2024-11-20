.data
     .globl x
     .align 4
    .type   x, @object
    .size   x, 4
x:
    .long 1
.text

inc:
    enter   $(8 * 2), $0 
    movq   %rdi, -8(%rbp)

    movq   -8(%rbp), %r10
    addq   -8(%rbp), %r10
    movq   %r10, -16(%rbp)

    mov   -16(%rbp), %eax 
    leave 
    ret
    leave 
    ret
.text

    .globl main

main:
    enter   $(8 * 6), $0 

    movl   $0, %r10d
    movl   %r10d, -8(%rbp)

    call   get_integer
    movl   %eax, -16(%rbp)

    movl   -16(%rbp), %r10d
    movl   %r10d, -8(%rbp)

    movl   -8(%rbp), %eax
    cmpl   $1, %eax
    sete   %al
    movzbl %al, %eax
    movl   %eax, -24(%rbp)

    movl   -24(%rbp), %eax
    cmpl   $0, %eax
    je     L0

    movq    -8(%rbp), %rdi
    call   printInteger
    movl   %eax, -32(%rbp)

    jmp    L1

L0:
    movq    -8(%rbp), %rdi
    call   inc
    movl   %eax, -40(%rbp)

    movq    -40(%rbp), %rdi
    call   printInteger
    movl   %eax, -48(%rbp)

    mov   -48(%rbp), %eax 
    leave 
    ret
L1:
    leave 
    ret
.ident   "GCC: (Ubuntu 13.2.0-23ubuntu4) 13.2.0" 
.section .note.GNU-stack,"",@progbits
