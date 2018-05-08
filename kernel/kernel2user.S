    .text
    .globl kernel2user
# Structure de la pile en entree:
#   %esp + 4: sommet de la pile user
#   %esp + 8: code processus
kernel2user:
    pushl 0x10;     # SS
    movl  4(%esp), %eax;
    pushl $0x80000000;     # SOMMET DE LA PILE USER
    pushf           # EFLAGS pushl 0x202
    pushl 0x18;     # CS
    movl  8(%esp), %eax;
    pushl $0x40000000;     # DÉBUT DU CODE PROCESSUS

    movl $0x4b, %ax;
    movl %ax, %ds;
    movl %ax, %es;
    movl %ax, %fs;
    movl %ax, %gs;
    iret
