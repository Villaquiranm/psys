    .text
    .globl cr3_sw

cr3_sw:
    movl 4(%esp), %eax
    movl %eax, %cr3
    movl %eax, tss+28
    ret
