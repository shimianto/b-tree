Esta pasta contém 6 testes toy. Os arquivos nomeados testN.txt foram feitos
de acordo com a sintaxe das operações usando apenas um índice. Os arquivos
testN_multiple.txt foram feitos de acordo com a sintaxe usando múltiplos 
índices, apesar de apenas um índice ser passado como parâmetro. Com exceção 
das sintaxes das operações, os testes são idênticos. 

É importante observar que as respostas não são únicas no caso da operação 
"dump", dependerá da implementação da árvore. Serão consideradas corretas 
as implementações que respeitem a ordenação das chaves, que mantenham 
as folhas na mesma altura e que respeitem a ordem da árvore, ou seja, todos 
os nós devem ter no máximo m chaves, e todos os nós exceto a raiz devem ter 
no mínimo (teto(m/2) - 1) chaves.

Para testar o seu programa, use as linhas de comando abaixo:

./main.exe ./my_answers/test1_myanswer.txt ./testes_toy/test1.txt 4 5 0
./main.exe ./my_answers/test2_myanswer.txt ./testes_toy/test2.txt 5 10 0
./main.exe ./my_answers/test3_myanswer.txt ./testes_toy/test3.txt 4 3 0 
./main.exe ./my_answers/test4_myanswer.txt ./testes_toy/test4.txt 6 3 0
./main.exe ./my_answers/test5_myanswer.txt ./testes_toy/test5.txt 4 3 0 
./main.exe ./my_answers/test6_myanswer.txt ./testes_toy/test6.txt 7 3 0

ou

./main.exe ./my_answers/test1_myanswer.txt ./testes_toy/test1_multiple.txt 4 5 0 
./main.exe ./my_answers/test2_myanswer.txt ./testes_toy/test2_multiple.txt 5 10 0 
./main.exe ./my_answers/test3_myanswer.txt ./testes_toy/test3_multiple.txt 4 3 0 
./main.exe ./my_answers/test4_myanswer.txt ./testes_toy/test4_multiple.txt 6 3 0 
./main.exe ./my_answers/test5_myanswer.txt ./testes_toy/test5_multiple.txt 4 3 0 
./main.exe ./my_answers/test6_myanswer.txt ./testes_toy/test6_multiple.txt 7 3 0 

