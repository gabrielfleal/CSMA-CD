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
int arrayPos[M_SIZE];

struct Transmissor{
	int id;
	int dado;
  int pos;
  int status; //0 - Sem dados; 1 - Com dados, aguardando o meio; 2 - Enviando;
  int idDestino;
};

void sendData(int idTransmissor){       //Adicionar destino

  if(!sensing() && hasData()){
    int posicaoT = arrayT[idTransmissor]->pos;
    meio[posicaoT] = arrayT[idTransmissor]->id;

    int i;
                sendHelp();
    for(i = 0; i < M_SIZE; i++){
      int r = M_SIZE - posicaoT + i;
      int l = posicaoT - i;

      if (r < M_SIZE) {
        meio[l] = arrayT[idTransmissor]->id;
      }
      if (r < M_SIZE) {
        meio[r] = arrayT[idTransmissor]->id;
      }

      view();

      fflush(stdout);
      sleep(1);
    }

  }
}

void sensing(){     //>>>>>>>>>>>>>AICIONAR FUNÇÃO PARA RECEBER O DADO CASO TENHA ALGO NA REDE E O DESTINATÁRIO SEJA SEU
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

void inicializaTransmissores(Transmissor array[]){
  int i, j;
  for (i = 0; i < N_TRANSMISSORES; i++) {
    array[i]->id = i;
  	array[i]->dado = 0;
    array[i]->status = 0;
    array[i]->idDestino = -1;

    do {
      int newPos = (random()% M_SIZE);
      int flagPos = 0;
      if(arrayPos[newPos]!=NULL){
        flagPos = 1;
      }
    } while(flagPos==1);

    array[i]->pos = newPos;
    arrayPos[array[i]->pos] = array[i]->id;
  }
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

  printf("\n\n          ");

  for (i = 0; i < M_SIZE; i++){
    if (arrayPos[i]!=NULL) {
      printf("%d ", arrayPos[i]);
    }else{
      printf("  ");
    }
  }

  printf("\n");
}

void inicializaMeio(){
  int i;
  for (i = 0; i < M_SIZE; i++) {
    meio[i] = 0;
    arrayPos[i] = NULL;
  }
}

main(){

  inicializaMeio();
  view();
  int opcao;
  Transmissor arrayT[N_TR];

  inicializaTransmissores(arrayT);

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
//To-Do
/*
Gerar dados aleatoriamente
Função hasData()
Adicionar destino pros dados
Conferir se há dados com destino para o meu id e gravar o dado
Testar colision (Adicionar função na hora de preencher o meio)
Enviar jams
Sensing - Testar se tem jams no meio
Backoff/tempo de espera
Threads
*/
