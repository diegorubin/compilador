#OBJETIVO: Ler uma string digitada pelo usuário
#
#ENTRADA: qualquer string que pode ser digitada
#
#OUTPUT: retorna o que foi digitado pelo usuário
#
#VARIÁVEIS:
# string = armazena a string digitada
# tam = armazena o tamanho da variável string
#
 
.section .data
string: .string "Digite algo:\n"
tam: .long . - string
 
.section .text
 
.globl _start
 
_start:
 
 
movl $4, %eax      # insere o valor 4, para a chamada da syscall write no EAX
movl $1, %ebx      # passa o parâmetro da syscall 4 para que algo seja exibido
leal string, %ecx   # carrega o endereço de memória do ECX e exibe o conteúdo de string
movl tam, %edx      # armazena o valor de tam no EDX
int $0x80      
 
movl %esp, %ecx    # Salva o Stack Pointer em %ecx
subl $10, %esp      # Reserva 10 bytes para o usuario digitar no stack
movl $3, %eax       # insere o valor da syscall read (3) no EAX, o que for escrito tbm será armazenado em EAX
movl $9, %edx       # Tamanho do que vai ser lido para EDX
int $0x80
 
movl %eax, %edx    # Move o que foi digitado para EDX.
movl $4, %eax      # syscall write
movl $1, %ebx
int $0x80
 
movl $0x1, %eax
movl $0x0, %ebx
int $0x80

