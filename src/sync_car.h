#ifndef SYNC_CAR_H
#define SYNC_CAR_H

#include <pthread.h>

struct station {
    pthread_mutex_t mutex;        //unica trava permitida
    pthread_cond_t  carroChegou;  //passageiros esperam aqui: carro presente com assento livre
    pthread_cond_t  carroPronto;  //carro espera aqui: embarque terminou
    int esperando;                //passageiros na estacao aguardando assento
    int assentosLivres;           //assentos ainda nao reservados no carro atual (0 se nao ha carro)
    int embarcando;               //passageiros liberados que ainda nao chamaram station_on_board
};

//Inicializa a estacao: zera contadores e inicializa a trava e as condicoes
void station_init(struct station *station);

//Chamada pelo carro quando chega na estacao com count assentos livres
void station_load_car(struct station *station, int count);

//Chamada pelo passageiro: so retorna quando ha carro na estacao com assento livre
void station_wait_for_car(struct station *station);

//Chamada pelo passageiro depois de embarcar e sentar
void station_on_board(struct station *station);

#endif