## MC 504 - Trabalho 1 
## Grupo:
 - Guilherme Ramirez - 217295
 - Pedro Pupo Alves - 204729
 - Victor Agozzini Scholze - 206457
-----
## Introdução
O objetivo deste trabalho é realizar a convolução de uma imagem com um kernel de blur de maneira multithread. 
## Estrutura de dados principal
A imagem é lida pela biblioteca stb_image.h e transformada em um vetor linear. 
Também definimos uma struct com os argumentos que serão utilizados para fazer a convolução da matriz.
## Estratégia para divisão de trabalho
Utilizamos as threads para fazer a convolução na matriz, para isso dividimos a matriz em blocos de linhas e passamos os limites de posição de cada bloco para as threads, que fazem a convolução de maneira paralela e colocam os novos valores em uma nova matriz
