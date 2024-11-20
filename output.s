.data
     .globl x
     .align 4
    .type   x, @object
    .size   x, 4
x:
    .long 0
.text

get_value:
    enter   $(8 * 0), $0 

    mov   $10, %eax 
    leave 
    ret
    leave 
    ret
.text

process_value:
    enter   $(8 * 13), $0 
    movq   %rdi, -8(%rbp)

    movl   -8(%rbp), %eax
    cltd
    movq   $3, %r10
    idivq  %r10
    movl   %edx, -24(%rbp)

    movl   -24(%rbp), %r10d
    movl   %r10d, -16(%rbp)

    movl   -8(%rbp), %eax
    cmpl   $5, %eax
    setg   %al
    movzbl %al, %eax
    movl   %eax, -32(%rbp)

    movl   -32(%rbp), %eax
    cmpl   $0, %eax
    je     L0

    movq   -8(%rbp), %r10
    imulq   $2, %r10
    movq   %r10, -48(%rbp)

    movl   -48(%rbp), %r10d
    movl   %r10d, x(%rip)

    jmp    L1

L0:
    movl   $2, %eax
    negl   %eax
    movl   %eax, -56(%rbp)

    movq   -8(%rbp), %r10
    subq   -56(%rbp), %r10
    movq   %r10, -64(%rbp)

    movl   -64(%rbp), %r10d
    movl   %r10d, x(%rip)

L1:
    movl   -16(%rbp), %eax
    cmpl   $0, %eax
    sete   %al
    movzbl %al, %eax
    movl   %eax, -72(%rbp)

    movl   -72(%rbp), %eax
    cmpl   $0, %eax
    je     L2

    movl   x(%rip), %eax
    cltd
    movq   $2, %r10
    idivq  %r10
    movl   %eax, -80(%rbp)

    movl   -80(%rbp), %r10d
    movl   %r10d, x(%rip)

    mov   $0, %eax 
    leave 
    ret
L2:
    movl   -16(%rbp), %eax
    cmpl   $2, %eax
    setl   %al
    movzbl %al, %eax
    movl   %eax, -88(%rbp)

    movl   -88(%rbp), %eax
    cmpl   $0, %eax
    je     L3

    movq   x(%rip), %r10
    addq   $1, %r10
    movq   %r10, -96(%rbp)

    movl   -96(%rbp), %r10d
    movl   %r10d, x(%rip)

    jmp    L4

L3:
    movq   x(%rip), %r10
    subq   $1, %r10
    movq   %r10, -104(%rbp)

    movl   -104(%rbp), %r10d
    movl   %r10d, x(%rip)

L4:
    leave
    ret

    call   get_value
    movl   %eax, -16(%rbp)

    movl   -16(%rbp), %r10d
    movl   %r10d, -8(%rbp)

    movq    -8(%rbp), %rdi
    call   process_value
    movl   %eax, -24(%rbp)

    movq    -8(%rbp), %rdi
    call   printInteger@PLT
    movl   %eax, -32(%rbp)
.ident   "GCC: (Ubuntu 13.2.0-23ubuntu4) 13.2.0" 
.section .note.GNU-stack,"",@progbits
