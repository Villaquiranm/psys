    .text
    .globl chprio

chprio:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $0, %esi
#on met des paramètres des fonctions dans des registres
movl 32(%esp), %ebx
movl 36(%esp), %ecx
#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
#-------------------------------------------------------------------------------
    .globl cons_write

cons_write:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $1, %esi
#on met des paramètres des fonctions dans des registres
movl 32(%esp), %ebx
movl 36(%esp), %ecx
#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
#-------------------------------------------------------------------------------
    .globl cons_read

cons_read:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $2, %esi

#on met des paramètres des fonctions dans des registres
#if defined CONS_READ_LINE
movl 32(%esp), %ebx
movl 36(%esp), %ecx
#endif
#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
#-------------------------------------------------------------------------------
    .globl cons_echo

cons_echo:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $3, %esi
#on met des paramètres des fonctions dans des registres
movl 32(%esp), %ebx
#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
#-------------------------------------------------------------------------------
    .globl exit

exit:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $4, %esi
#on met des paramètres des fonctions dans des registres
movl 32(%esp), %ebx
#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
#-------------------------------------------------------------------------------
    .globl getpid

getpid:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $5, %esi
#on met des paramètres des fonctions dans des registres
movl 32(%esp), %ebx
#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
#-------------------------------------------------------------------------------
    .globl getprio

getprio:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $6, %esi
#on met des paramètres des fonctions dans des registres
movl 32(%esp), %ebx
#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
#-------------------------------------------------------------------------------
    .globl kill

kill:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $7, %esi
#on met des paramètres des fonctions dans des registres
movl 32(%esp), %ebx
#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
#-------------------------------------------------------------------------------
    .globl pcount

pcount:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $8, %esi
#on met des paramètres des fonctions dans des registres
movl 32(%esp), %ebx
movl 36(%esp), %ecx
#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
#-------------------------------------------------------------------------------
    .globl pcreate

pcreate:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $9, %esi
#on met des paramètres des fonctions dans des registres
movl 32(%esp), %ebx
#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
#-------------------------------------------------------------------------------
    .globl pdelete

pdelete:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $10, %esi
#on met des paramètres des fonctions dans des registres
movl 32(%esp), %ebx
#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
#-------------------------------------------------------------------------------
    .globl preceive

preceive:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $11, %esi
#on met des paramètres des fonctions dans des registres
movl 32(%esp), %ebx
movl 36(%esp), %ecx
#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
#-------------------------------------------------------------------------------
    .globl preset

preset:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $12, %esi
#on met des paramètres des fonctions dans des registres
movl 32(%esp), %ebx
#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
#-------------------------------------------------------------------------------
    .globl psend

psend:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $13, %esi
#on met des paramètres des fonctions dans des registres
movl 32(%esp), %ebx
movl 36(%esp), %ecx
#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
#-------------------------------------------------------------------------------
.globl clock_settings

clock_settings:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $14, %esi
#on met des paramètres des fonctions dans des registres
movl 32(%esp), %ebx
movl 36(%esp), %ecx
#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
#-------------------------------------------------------------------------------
.globl current_clock

current_clock:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $15, %esi
#on met des paramètres des fonctions dans des registres

#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
#-------------------------------------------------------------------------------
.globl wait_clock

wait_clock:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $16, %esi
#on met des paramètres des fonctions dans des registres
movl 32(%esp), %ebx
#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
#-------------------------------------------------------------------------------
.globl start

start:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $17, %esi
#on met des paramètres des fonctions dans des registres
movl 32(%esp), %ebx
movl 36(%esp), %ecx
movl 40(%esp), %edx
movl 44(%esp), %edi
#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
#-------------------------------------------------------------------------------
.globl waitpid

waitpid:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $18, %esi
#on met des paramètres des fonctions dans des registres
movl 32(%esp), %ebx
movl 36(%esp), %ecx
#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
#-------------------------------------------------------------------------------
.globl sys_info

sys_info:
#on sauvegarde des valeurs des registres
pushl %ebx
pushl %ecx
pushl %edx
pushl %edi
pushl %esi
pushl %ebp
pushl %esp
#on met dans le registre eax la valeur correspondente de l'appel
movl $19, %esi
#on met des paramètres des fonctions dans des registres
movl 32(%esp), %ebx
movl 36(%esp), %ecx
#interruption
int $49
#restauration des registres
popl %esp
popl %ebp
popl %esi
popl %edi
popl %edx
popl %ecx
popl %ebx
ret
