    .text
    .globl ctx_sw
# Structure de la pile en entree :
#   %esp + 4 : adresse de l'ancien contexte
#   %esp + 8 : adresse du nouveau contexte
ctx_sw:
    # sauvegarde du contexte de l'ancien processus
    movl 4(%esp), %eax
    movl %ebx, (%eax)
    movl %esp, 4(%eax)
    movl %ebp, 8(%eax)
    movl %esi, 12(%eax)
    movl %edi, 16(%eax)

    # restauration du contexte du nouveau processus
    movl 8(%esp), %eax
    movl (%eax), %ebx
    movl 4(%eax), %esp
    movl 8(%eax), %ebp
    movl 12(%eax), %esi
    movl 16(%eax), %edi

    # on passe la main au nouveau processus

    #TODO le processeur va obligatoirement changer de pointeur de pile, c'est pourquoi il faut 2 piles par processus (fonctionnement nécessaire pour des questions de sécurité à mon avis). Le processeur va mettre dans ESP l'adresse qu'il trouve dans la TSS+4 (c'est à dire à l'adresse 0x20004) vous devez donc simplement modifier le contenu de cette zone mémoire à chaque Context Switch. Vous devez également mettre 0x18 à l'adresse 0x20008 car le processeur change également le registre SS en switchant de pile.

    #TODO ici faire des pushes qu'il faut pour garantir le mode user!
    mov $0x10, %ax  #kernel mode,  pour user mode: $0x4b
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    mov %esp, %eax
    pushl $0x10     #kernel mode,  pour user mode: $0x4b
    pushl %eax
    pushf
    pushl $0x18    #kernel mode,   pour user mode: $0x43
    push $1f    #TODO mettre eip, l'adresse du debut du processus a executer

    iret
