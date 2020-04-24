# e possivel definir variaveis, como o compilador e flags de compilacao
# o flag -g serve para usar o gdb e o valgrind, e o flag -O0 impede que 
# o compilador otimize o codigo
CC = gcc
CFLAGS = -g -O0 

# a primeira regra sera executada, e nesse caso, ela depende das regras 
# multiplicador e somador, que vao gerar um executavel cada.
# as operacoes foram divididas em executaveis diferentes apenas para 
# exemplificar um makefile com dois executaveis
all: main.exe

main.exe: main.o b_tree.o queue.o
	$(CC) $(CFLAGS) main.o b_tree.o queue.o -o main.exe

main.o: main.c b_tree.h queue.h
	$(CC) $(CFLAGS) -c main.c

# fazer as regras multiplicador e somador dependerem de operacoes.o 
# evita a recompilacao de operacoes

b_tree.o: b_tree.h b_tree.c
	$(CC) $(CFLAGS) -c b_tree.c 

queue.o: queue.h queue.c
	$(CC) $(CFLAGS) -c queue.c 

# essa regra limpa os arquivos gerados durante todas as compilacoes.
# rm e um comando linux para apagar arquivos definitivamente. tome 
# cuidado no uso de wildcards (*)
clean:
	rm *.o main.exe

cleantxt:
	rm *.txt
