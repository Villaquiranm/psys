    .text
    .globl ret_user

    ret_user:
        pushl %ebx
        pushl %ecx
        pushl %edx
        pushl %edi
        pushl %esi
        pushl %ebp
        pushl %esp
        movl $4, %esi
        movl %eax, %ebx
        int $49
        popl %esp
        popl %ebp
        popl %esi
        popl %edi
        popl %edx
        popl %ecx
        popl %ebx
        ret
