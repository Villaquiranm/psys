    .text
# cette directive sert a rendre l'etiquette publique
    .globl traitant_IT_32
# debut du traitant
traitant_IT_32:
# sauvegarde des registres importants
    pushl %eax
    pushl %edx
    pushl %ecx
# appel a la fonction C realisant le traitant
# a decommenter quand vous l'aurez implantee
    call tic_PIT
# restauration des registres importants
    popl %ecx
    popl %edx
    popl %eax
# fin du traitant
    iret

# cette directive sert a rendre l'etiquette publique
    .globl traitant_IT_49
# debut du traitant
traitant_IT_49:
# sauvegarde des registres
    pushl %ebx
    movl $0x18, %ebx
    movl %ebx, %ds
    movl %ebx, %es
    movl %ebx, %fs
    movl %ebx, %gs
    popl %ebx

    pushl %esi
    pushl %ebx
    pushl %ecx
    pushl %edx
    pushl %edi
    pushl %ebp
    pushl %esp
# appel a la fonction C realisant le traitant
    call traitant_appels_systeme
# restauration des registres
    popl %esp
    popl %ebp
    popl %edi
    popl %edx
    popl %ecx
    popl %ebx
    popl %esi

    pushl %ebx
    movl $0x4b, %ebx
    movl %ebx, %ds
    movl %ebx, %es
    movl %ebx, %fs
    movl %ebx, %gs
    popl %ebx
# On n'ecrit pas sur EAX puisqu'il est utilise pour le retour
# fin du traitant
    iret
#------------------------------------------------------------
.text
# cette directive sert a rendre l'etiquette publique
.globl traitant_IT_33
# debut du traitant
traitant_IT_33:
# sauvegarde des registres importants
pushl %eax
pushl %edx
pushl %ecx
# appel a la fonction C realisant le traitant
# a decommenter quand vous l'aurez implantee
call it_clavier
# restauration des registres importants
popl %ecx
popl %edx
popl %eax
# fin du traitant
iret
