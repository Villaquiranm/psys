.text
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
movl $1, %eax
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
################################################################################
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
movl $2, %eax
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
################################################################################
.text
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
movl $3, %eax
#on met des paramètres des fonctions dans des registres
movl 4(%esp), %ebx
movl 8(%esp), %ecx
movl 12(%esp), %edx
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
################################################################################
.text
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
movl $4, %eax

#on met des paramètres des fonctions dans des registres
#if defined CONS_READ_LINE
movl 4(%esp), %ebx
movl 8(%esp), %ecx
movl 12(%esp), %edx

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
################################################################################
.text
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
################################################################################
.text
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
################################################################################
.text
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
################################################################################
.text
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
movl $8, %eax
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
################################################################################
.text
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
movl $9, %eax
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
################################################################################
.text
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
################################################################################
.text
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
movl $11, %eax
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
################################################################################
.text
.globl preceive

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
movl $12, %eax
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
################################################################################
.text
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
movl $13, %eax
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
################################################################################
.text
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
################################################################################
