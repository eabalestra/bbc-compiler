.file "output.s"
.text

inc:
    enter   $(8 * 2), $0 
    movq   %rdi, -8(%rbp)

    movq   -8(%rbp), %r10
    addq   $1, %r10
    movq   %r10, -16(%rbp)

    mov   -16(%rbp), %rax 
    leave 
    ret

    .globl main
main:
    enter   $(8 * 7), $0 

    call   get_integer@PLT
    movl   %eax, -16(%rbp)
    movq    -16(%rbp), %rdi
    call   inc
    movl   %eax, -24(%rbp)

    movq   -24(%rbp), %r10
    movq   %r10, -8(%rbp)

    movq    -8(%rbp), %rdi
    call   printInteger@PLT
    movl   %eax, -32(%rbp)
    movq   -8(%rbp), %r10
    cmpq   $2, %r10
    sete   %al
    movzbl %al, %eax
    movl   %eax, -40(%rbp)

    cmpl   $0, -40(%rbp)
    je     L0

    movq    $1, %rdi
    call   printInteger@PLT
    movl   %eax, -48(%rbp)
    jmp    L1

L0:
    movq    $0, %rdi
    call   printInteger@PLT
    movl   %eax, -56(%rbp)
L1:
    mov   $0, %rax 
    leave 
    ret
.ident   "GCC: (Ubuntu 13.2.0-23ubuntu4) 13.2.0" 
.section .note.GNU-stack,"",@progbits
