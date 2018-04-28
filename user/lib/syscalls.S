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
movl $0, %eax
#on met des paramètres des fonctions dans des registres
movl 4(%esp), %ebx
movl 8(%esp), %ecx
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
movl $1, %eax
#on met des paramètres des fonctions dans des registres
movl 4(%esp), %ebx
movl 8(%esp), %ecx
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
movl $2, %eax

#on met des paramètres des fonctions dans des registres
#if defined CONS_READ_LINE
movl 4(%esp), %ebx
movl 8(%esp), %ecx
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
movl $3, %eax
#on met des paramètres des fonctions dans des registres
movl 4(%esp), %ebx
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
movl $4, %eax
#on met des paramètres des fonctions dans des registres
movl 4(%esp), %ebx
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
movl $5, %eax
#on met des paramètres des fonctions dans des registres
movl 4(%esp), %ebx
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
movl $6, %eax
#on met des paramètres des fonctions dans des registres
movl 4(%esp), %ebx
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
movl $7, %eax
#on met des paramètres des fonctions dans des registres
movl 4(%esp), %ebx
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
movl $8, %eax
#on met des paramètres des fonctions dans des registres
movl 4(%esp), %ebx
movl 8(%esp), %ecx
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
movl $9, %eax
#on met des paramètres des fonctions dans des registres
movl 4(%esp), %ebx
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
movl $10, %eax
#on met des paramètres des fonctions dans des registres
movl 4(%esp), %ebx
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
movl $11, %eax
#on met des paramètres des fonctions dans des registres
movl 4(%esp), %ebx
movl 8(%esp), %ecx
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
movl $12, %eax
#on met des paramètres des fonctions dans des registres
movl 4(%esp), %ebx
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
movl $13, %eax
#on met des paramètres des fonctions dans des registres
movl 4(%esp), %ebx
movl 8(%esp), %ecx
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
movl $14, %eax
#on met des paramètres des fonctions dans des registres
movl 4(%esp), %ebx
movl 8(%esp), %ecx
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
movl $15, %eax
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
movl $16, %eax
#on met des paramètres des fonctions dans des registres
movl 4(%esp), %ebx
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
movl $17, %eax
#on met des paramètres des fonctions dans des registres
movl 4(%esp), %ebx
movl 8(%esp), %ecx
movl 12(%esp), %edx
movl 16(%esp), %edi
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
movl $18, %eax
#on met des paramètres des fonctions dans des registres
movl 4(%esp), %ebx
movl 8(%esp), %ecx
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
movl $19, %eax
#on met des paramètres des fonctions dans des registres
movl 4(%esp), %ebx
movl 8(%esp), %ecx
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
