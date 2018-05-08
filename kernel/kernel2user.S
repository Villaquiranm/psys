    .text
    .globl kernel2user
# Structure de la pile en entree:
#   %esp + 4: pile user
#   %esp + 8: code processus
kernel2user:
    pushl 0x10;     # SS
    movl  4(%esp), %eax;
    pushl %eax;     # PILE USER
    pushl 0x202;    # EFLAGS
    pushl 0x18;     # CS
    movl  8(%esp), %eax;
    pushl %eax;     # DÉBUT DU CODE PROCESSUS

    movl $0x4b, %ax;
    movl %ax, %ds;
    movl %ax, %es;
    movl %ax, %fs;
    movl %ax, %gs;
    iret
