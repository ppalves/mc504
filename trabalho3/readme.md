## MC 504 - Trabalho 3
## Grupo:
 - Guilherme Ramirez - 217295
 - Pedro Pupo Alves - 204729
 - Victor Agozzini Scholze - 206457
-----
## Introdução
O objetivo deste trabalho é a implementação de uma aplicação multithread que utiliza locks ou variáveis de condição para sincronização e de um visualizador para o progresso desta aplicação.
## O problema escolhido
Escolhemos o problema "River crossing", descrito na página 148 do livro "The Little Book of Semaphores" de Allen B. Downey. O problema consiste em duas threads, uma de hackers Linux e outra de empregados da Microsoft (serfs). Eles devem cruzar o rio em um barco com 4 lugares, com a seguinte restrição, um hacker não pode ir com três serfs e três hackers não podem ir com um serf, todas as outras posibilidades são seguras. Todos que embarcarem no barco devem sinalizar que embarcaram e, assim que as quatro pessoas sinalizarem que e embarcaram, uma delas deve sinalizar para o barco zarpar, não importando quem sinaliza.

## Implementação
Usamos como base o pseudocódigo fornecido pelo livro, ou seja, dois semáforos, um para os hackers e outro para os serfs, um mutex e uma barreira. As threads dos hackers e serfs sao espelhada, sendo assim vamos usar a thread dos hackers para ilustrar o processo. Quando entramos na thread, essa da um lock no mutex, após obter o lock aumentamos em um o número de hackers. Em seguida verificamos se é possivel formar um barco com os hackers e serfs que temos até aqui. Vamos quebrar esse fluxo em 2 casos:

- É possivel formar um barco: Nesse caso vamos marcar a thread atual como thread capitã e remover o número de serfs e hackers q entraram no barco das variaveis compartilhadas. Além disso vamos sinalizar para os semafaros de serfs e hackers que novas pessoas embarcaram

- Não é possivel formar um barco: Nesse caso, a thread simplesmente libera o lock do mutex

Em seguida chegamos na parte em que o semaforo espera. Caso a thread não seja a capitã ela vai esperar nessa etapa. Se for uma thread capitã ela vai ter acabado de sinalizar para as outras threads (e ela propria) que pode processeguir pelo semaforo.

Após passar pelo semaforo, os hackers e serfs embarcam. Nessa etapa usamos um outro mutex para controlar o acesso a variavel que indica a quantidade de serfs e hackers q estão esperando no barco para sair. 

Após isso temos uma barreira e após as quatro threads chegarem na barreira a execução continua.

Por ultimo, a thread capitã manda o barco sair e libera o mutex.

Para executar os prints da animação utilizamos um outro mutex para contolar o acesso a zona critica.
