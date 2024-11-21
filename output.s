.data
     .globl x
     .align 4
    .type   x, @object
    .size   x, 4
x:
    .long 1
     .globl y
     .align 4
    .type   y, @object
    .size   y, 4
y:
    .long 1
.text

f:
    enter   $(8 * 6), $0 
    movq   %rdi, -8(%rbp)

    movl   -8(%rbp), %eax
    cmpl   $1, %eax
    sete   %al
    movzbl %al, %eax
    movl   %eax, -24(%rbp)

    movl   -24(%rbp), %eax
    cmpl   $0, %eax
    je     L0

    mov   $1, %eax 
    leave 
    ret
    jmp    L1

L0:
    movq   -8(%rbp), %r10
    subq   $1, %r10
    movq   %r10, -32(%rbp)

    movq    -32(%rbp), %rdi
    call   f
    movl   %eax, -40(%rbp)

    movq   -8(%rbp), %r10
    imulq   -40(%rbp), %r10
    movq   %r10, -48(%rbp)

    mov   -48(%rbp), %eax 
    leave 
    ret
L1:
    leave 
    ret
.text

metodo:
    enter   $(8 * 8), $0 
    movq   %rdi, -8(%rbp)
    movq   %rsi, -16(%rbp)

    movl   -16(%rbp), %eax
    cmpl   $4, %eax
    setg   %al
    movzbl %al, %eax
    movl   %eax, -24(%rbp)

    movl   -24(%rbp), %eax
    cmpl   $0, %eax
    je     L2

    movq    y(%rip), %rdi
    movq   -16(%rbp), %r10
    subq   $1, %r10
    movq   %r10, -48(%rbp)

    movq    -48(%rbp), %rsi
    call   metodo
    movl   %eax, -56(%rbp)

    movq   x(%rip), %r10
    addq   -56(%rbp), %r10
    movq   %r10, -64(%rbp)

    movl   -64(%rbp), %r10d
    movl   %r10d, x(%rip)

L2:
    mov   x(%rip), %eax 
    leave 
    ret
    leave 
    ret
.text

meth_no_param:
    enter   $(8 * 4), $0 

    movq   $1, %r10
    imulq   x(%rip), %r10
    movq   %r10, -16(%rbp)

    movq   x(%rip), %r10
    addq   x(%rip), %r10
    movq   %r10, -24(%rbp)

    movq   -16(%rbp), %r10
    addq   -24(%rbp), %r10
    movq   %r10, -32(%rbp)

    mov   -32(%rbp), %eax 
    leave 
    ret
    leave 
    ret
.text

    .globl main

main:
    enter   $(8 * 8), $0 

    movl   $0, %r10d
    movl   %r10d, -16(%rbp)

    call   meth_no_param
    movl   %eax, -32(%rbp)

    movl   -32(%rbp), %r10d
    movl   %r10d, -24(%rbp)

    movl   $1, %eax
    cmpl   $0, %eax
    sete   %al
    movzbl %al, %eax
    movl   %eax, -48(%rbp)

    movq    -48(%rbp), %rdi
    movq    $7, %rsi
    call   metodo
    movl   %eax, -56(%rbp)

    movl   -56(%rbp), %r10d
    movl   %r10d, -40(%rbp)

    movq    -40(%rbp), %rdi
    call   printInteger
    movl   %eax, -64(%rbp)

    leave 
    ret
.ident   "GCC: (Ubuntu 13.2.0-23ubuntu4) 13.2.0" 
.section .note.GNU-stack,"",@progbits
