# Sync METROREC

Sincronização do metrô da METROREC feito em C pra atividade de Infraestrutura de Software (Implementação 5). Cada vagão de metrô é uma thread e cada passageiro também; o programa coordena o embarque usando só um mutex e variáveis de condição do pthread (sem semáforos e sem busy-wait).

## Arquivos do projeto

- `src/sync_car.h` — header com a struct station (mutex, condições e contadores) e os protótipos das quatro funções
- `src/sync_car.c` — implementação de station_init, station_load_car, station_wait_for_car e station_on_board
- `src/sync_car-runner.c` — programa de teste fornecido pelo professor (contém o main); imprime "Looks good!" quando a sincronização está correta

## Como compilar

```
make
```
</br></br>
Isso gera o executável sync_car na raiz.

```
make tsan
```
</br></br>
Gera o sync_car_tsan, versão compilada com ThreadSanitizer pra detectar data race.

```
make clean
```
Pra limpar os .o e os executáveis (seja o sync_car ou sync_car_tsan).

#### O warning de unused parameter 'foo' que aparece ao compilar vem do sync_car-runner.c (arquivo do classroom) e não do código que eu desenvolvi, então ignorei.

## Como executar

```
./sync_car
```
</br></br>
A saída mostra cada carro entrando e saindo da estação com a quantidade de passageiros embarcados. Se os 100 passageiros embarcarem corretamente, termina com "Looks good!".

## Testes

O teste disponibilizado no classroom é probabilístico, então além da execução única também foi decidido testar rodando várias vezes seguidas:

```
for i in $(seq 1 200); do ./sync_car > /dev/null 2>&1 || echo "FALHOU na execucao $i"; done
```
</br></br>
Se nada for impresso, todas as 200 execuções terminaram com "Looks good!".

Pra checar concorrência:

```
make tsan && ./sync_car_tsan
```
</br></br>
O ThreadSanitizer aponta data race nas variáveis globais do runner (threads_completed, load_car_returned), que o professor usa sem lock de propósito, novamente envolvendo o código posto no classroom e não o código que desenvolvi. Por isso, nenhum aviso deve envolver a "struct station".

## Arquitetura de arquivos
```
├── src/
│   ├── sync_car.h
│   ├── sync_car.c
│   └── sync_car-runner.c
├── Makefile
└── README.md
```

## Sistema Operacional

Esse projeto foi feito pelo Linux, mais especificamente na distro Omarchy (Arch Linux). A implementação utiliza exclusivamente as funções pthread listadas na especificação (pthread_mutex_* e pthread_cond_*), o que garante compatibilidade com qualquer sistema que siga o padrão POSIX, incluindo outras distribuições Linux, macOS e sistemas Unix em geral. A compilação foi feita com o GCC usando as flags -Wall -Wextra -O2 -pthread.
