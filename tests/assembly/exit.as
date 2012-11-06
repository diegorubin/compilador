#OBJETIVO: Programa simples que executa um exit e retorna um código de status para o kernel Linux
 #
 #ENTRADA: nenhuma
 #
 #OUTPUT: retorna um status código de status, que pode ser visto executando no terminal o comando:
 #
 # echo $?
 #
 # após a execução do programa
 #
 #VARIÁVEIS:
 # %eax armazena o número da syscall
 # %ebx armazena o status retornado
 #
 
 .section .data
 
 .section .text
 
 .globl _start
 
 _start:
 
 movl $1, %eax       # esta é a syscall do kernel Linux para sair de um programa
 movl $0, %ebx       # este é o status que retornaremos para o SO.
         # altere esse valor, e verá coisas diferentes ao executar o
         # echo $?
 int $0x80       # isso chama o kernel para executar a syscall 1

