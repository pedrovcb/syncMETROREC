#include <pthread.h>

#include "sync_car.h"

//Inicializa a estacao: zera contadores e inicializa a trava e as condicoes
void station_init(struct station *station){
    pthread_mutex_init(&station->mutex, NULL);
    pthread_cond_init(&station->carroChegou, NULL);
    pthread_cond_init(&station->carroPronto, NULL);

    station->esperando = 0;
    station->assentosLivres = 0;
    station->embarcando = 0;
}

//Passageiro chegou: espera ate ter carro na estacao com assento livre
void station_wait_for_car(struct station *station){
    pthread_mutex_lock(&station->mutex);

    station->esperando++;

    //enquanto nao ha carro ou o carro esta cheio, dorme
    while(station->assentosLivres == 0){
        pthread_cond_wait(&station->carroChegou, &station->mutex);
    }

    station->esperando--;
    station->assentosLivres--; //reserva o assento pro passageiro
    station->embarcando++;

    pthread_mutex_unlock(&station->mutex);
}

//Passageiro sentou: avisa o carro que mais um embarcou
void station_on_board(struct station *station){
    pthread_mutex_lock(&station->mutex);

    station->embarcando--;

    //se nao tem mais ninguem embarcando e o carro pode partir, acorda o carro
    if(station->embarcando == 0 && (station->esperando == 0 || station->assentosLivres == 0)){
        pthread_cond_signal(&station->carroPronto);
    }

    pthread_mutex_unlock(&station->mutex);
}

//Carro chegou com count assentos livres: libera passageiros e so parte quando satisfatorio
void station_load_car(struct station *station, int count){
    pthread_mutex_lock(&station->mutex);

    if(count > 0 && station->esperando > 0){
        station->assentosLivres = count;

        //acorda todos os passageiros; cada um pega um assento enquanto houver
        pthread_cond_broadcast(&station->carroChegou);

        //espera enquanto ainda ha passageiro com assento pra embarcar, ou gente embarcando
        while((station->esperando > 0 && station->assentosLivres > 0) || station->embarcando > 0){
            pthread_cond_wait(&station->carroPronto, &station->mutex);
        }
    }

    //carro parte: quem chegar depois espera o proximo
    station->assentosLivres = 0;

    pthread_mutex_unlock(&station->mutex);
}