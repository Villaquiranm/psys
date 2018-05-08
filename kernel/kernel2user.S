    .text
    .globl kernel2user
# Structure de la pile en entree:
#   %eax: sommet de la pile user
#   %eax + 4: code processus
kernel2user:

    movl %esp, %eax

    pushl $0x18;            # SS
    pushl (%eax);           # SOMMET DE LA PILE USER
    pushf                   # EFLAGS pushl 0x202
    pushl $0x10;            # CS
    pushl 4(%eax);          # DÉBUT DU CODE PROCESSUS

    movl $0x18, %eax;
    movl %eax, %ds;
    movl %eax, %es;
    movl %eax, %fs;
    movl %eax, %gs;
    iret
