## MC 504 - Trabalho 2 
## Grupo:
 - Guilherme Ramirez - 217295
 - Pedro Pupo Alves - 204729
 - Victor Agozzini Scholze - 206457
-----
## Introdução
O objetivo deste trabalho é implementar e observar um problema clássico de sincronização de um processo multithread com o uso de semáforos e criar uma animação para facilitar a visualização.
## O problema escolhido
Escolhemos o problema do "Construindo H20", descrito na página 143 do livro "The Little Book of Semaphores" de Allen B. Downey. O problema consiste em duas threads, uma que produz oxigênio e outra que produz hidrogênio, tal que, devemos criar uma molécula de água com dois hidrogênios e um oxigênio. Para isso, deve-se criar uma barreira, fazendo a outra thread esperar até que uma molécula esteja pronta para prosseguir. Devemos garantir que todas as threads da molécula chame bond() antes das outras threads da próxima molécula.
## Implementação
Usamos como base o pseudocódigo fornecido pelo livro, ou seja, dois semáforos, um para hidrogênio e outro para oxigênio, um mutex e uma barreira. Na thread do oxigênio pega-se o mutex, gera um oxigênio e verifica se existe hidrogênios suficientes para formar uma água, caso exista ele sinaliza para o semáforo de oxigênio e para o hidrogênio. Em seguida espera o semáforo do oxigênio, envoca bond(), espera a barreira e solta o mutex.
