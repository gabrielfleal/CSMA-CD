/*
O trabalho consiste em desenvolver uma simulação para o controle de acesso ao  meio utilizado em redes IEEE 802.3.
A implementação deve possuir no mínimo dois transmissores que utilizam a tecnologia citada. Cada transmissor antes de enviar
um quadro deverá verificar (sensing), por um determinado intervalo de tempo, se o meio de transmissão está ocupado, ou ocorreu colisões.
Caso o meio de transmissão estiver ocupado, deve-se implementar o algoritmo de backoff, isto é, esperar um tempo exponencial truncado.

Forma de Avaliação:
1. Implementação dos transmissores – valor 1
2. Definição, verificação e implementação da ocupação do canal - valor 2
3. Definição e detecção de colisões - valor 2
4. Implementação do algoritmo backoff - valor 2
5. Visualização dos quadros sendo transmitido– valor 2
6. Apresentação do trabalho - valor 1
*/

#include<semaphore.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define N_TRANSMISSORES 2
#define M_SIZE 10

int meio[M_SIZE];
int colision = 0;
int usingMedium = 0;
int defaultTime = 10;

struct Transmissor{
	int id;
	int dado;
  int pos;
  int status;
  int idDestino;
};

void sendData(int idTransmissor){

  if(sensing() && hasData()){
    int posicaoT = arrayT[idTransmissor]->pos;
    meio[posicaoT] = arrayT[idTransmissor]->id;
    for(int i = posicaoT; i < M_SIZE; i++){
      meio[posicaoT] = arrayT[idTransmissor]->id;
      fflush(stdout);
      sleep(1);
    }
  }
}

void sensing(){
  int flagSensing = 0;
  for(i = 0; i < sizeof(M_SIZE); i++){
    if(meio[i]!=NULL){
      flagSensing = 1;
    }
  }
  return flagSensing;
}

void colision(){

}

void backoff(){
  int tempoEspera = 0;
  int randTime = (random()%2);

}

void view(){
  int i;
  printf("\n --- Meio de transmissão --- \n");
  printf("\nPosição:  ");
  for(i = 0; i < M_SIZE; i++)
    printf("%d ", i);

  printf("\n\n          ");

  for (i = 0; i < M_SIZE; i++)
    printf("%d ", meio[i]);

  printf("\n");
}

void inicializaMeio(){
  int i;
  for (i = 0; i < M_SIZE; i++) {
    meio[i] = 0;
  }
}

main(){

  inicializaMeio();
  view();
  int opcao;
  Transmissor arrayT[N_TR];

	pthread_t thread[N_TR];
	void *thread_result;
  // pthread_mutex_init(&mutex, NULL);
	// for(i=0; i<N; i++)
	// 	sem_init(&sem_transmissores[i], 0, 0);
  //
    do{
    	printf("Escolha uma das opções abaixo:\n1 - Iniciar simulação;\n0 - Encerrar;\n");
    	scanf("%d", &opcao);
  		switch(opcao){
        case 1:
            int array[N_TR];
            for(i=0; i<N_TR; i++){
                // array[i]=i;
  //               pthread_create(&thread[i], NULL, acao_transmissor, &array[i]);
            }
  //           for(i=0; i<N; i++)
  //               pthread_join(thread[i], &thread_result);
        case 0:
            printf("\nFim da execução.\n");
            break;
        default:
            printf("Erro! Digite novamente");
        }
    } while(opcao!=0);
	return 0;
}
