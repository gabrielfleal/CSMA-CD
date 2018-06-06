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
#define MAX_COLISIONS 50

int meio[M_SIZE];
int nColision = 0;
int defaultTime = 10;
int arrayPos[M_SIZE];

struct Transmissor{
	int id;
	int dado;
  int pos;
  int status; //0 - Sem dados; 1 - Com dados, aguardando o meio; 2 - Enviando;
  int idDestino;
  int dadoRecebido;
};
typedef struct Transmissor Transmissor;

Transmissor arrayT[N_TRANSMISSORES];

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
    if (&arrayPos[i] != NULL) {
      printf("%d ", arrayPos[i]);
    }else{
      printf("  ");
    }
  }

  printf("\n");
}

void limparMeio(int idTransmissor){
  int posInicio = arrayT[idTransmissor].pos;
  int i;
  for(i = 0; i < M_SIZE; i++){
    int r = M_SIZE - posInicio + i;
    int l = posInicio - i;

    if (l >= 0 && meio[l] != arrayT[idTransmissor].id) {
      meio[l] = -1;
    }
    if (r < M_SIZE && meio[r] != arrayT[idTransmissor].id) {
      meio[r] = -1;
    }
    view();
    fflush(stdout);
    sleep(1);
  }
}

void enviarJam(){
  
}

int detectarColisao(int pos, int idTransmissor){
  int flagColisao = 0;
  if(meio[pos] != -1 && meio[pos] != idTransmissor){
    flagColisao = 1;
    printf("\n----- Colisão na posição: %d\n", pos);
    enviarJam();
  }
  return flagColisao;
}

void sendData(int idTransmissor){
  if(!sensing(idTransmissor) && possuiDado(idTransmissor)){
    arrayT[idTransmissor].status = 2;
    int posicaoT = arrayT[idTransmissor].pos;
    meio[posicaoT] = arrayT[idTransmissor].id;

    int i;
    for(i = 0; i < M_SIZE; i++){
      int r = M_SIZE - posicaoT + i;
      int l = posicaoT - i;

      if (l >= 0) {
        if(detectarColisao(l, idTransmissor)){
            break;
        }else{
          meio[l] = arrayT[idTransmissor].id;
        }
      }

      if (r < M_SIZE) {
        if(detectarColisao(r, idTransmissor)){
            break;
        }else{
          meio[r] = arrayT[idTransmissor].id;
        }
      }

      view();
      fflush(stdout);
      sleep(1);
    }
  }
  arrayT[idTransmissor].status = 0; //O status do transmissor volta a ficar livre e o dado enviado e seu destino são apagados
  arrayT[idTransmissor].dado = -1;
  arrayT[idTransmissor].idDestino = -1;

  limparMeio(idTransmissor);
}

int possuiDado(int idTransmissor){
  int flagDado = 0;
  if(arrayT[idTransmissor].status != 0){
    flagDado = 1;
  }
  return flagDado;
}

int sensing(int idTransmissor){
  int flagSensing;
  int i;
  do{
    sleep(3); //Tempo para verificar se o meio está ocupado
    flagSensing = 0;
    for(i = 0; i < sizeof(M_SIZE); i++){
      if(meio[i] != -1){ //O meio está ocupado
        flagSensing = 1;
        if(i == arrayT[idTransmissor].pos){
          if(meio[i] == -2){ //O sinal -2 mostra que ocorreu colision e o jam chegou na posição deste transmissor
            sleep(3);
          }else
          if(arrayT[meio[i]].idDestino == idTransmissor){ //O meio possui sinal que não é indicador colision -> Testar se o dado transmitido possui como destino o id do Transmissor que está verificando o meio
            arrayT[idTransmissor].dadoRecebido = arrayT[meio[i]].dado;
            sleep(1);
          }
        }
      }
    }
  }while(flagSensing==1);
  arrayT[idTransmissor].dadoRecebido = -1;

  return flagSensing;
}

void backoff(){
  int tempoEspera = 0;
  int randTime = (random()%2);
}

void inicializaTransmissores(Transmissor array[]){
  int i, j;
  for (i = 0; i < N_TRANSMISSORES; i++) {
    array[i].id = i;
  	array[i].dado = -1;
    array[i].status = 0;
    array[i].idDestino = -1;
    array[i].dadoRecebido = -1;

    int flagPos = 0;
    int newPos;
    do {
      newPos = (int)(random()% M_SIZE-1);
      flagPos = 0;
      if(arrayPos[newPos]!= -1){
        flagPos = 1;
      }
    } while(flagPos==1);

    array[i].pos = newPos;
    arrayPos[array[i].pos] = array[i].id;
  }
}

void inicializaMeio(){
  int i;
  for (i = 0; i < M_SIZE; i++) {
    meio[i] = 0;
    arrayPos[i] = -1;
  }
}

void geraDado(int idTransmissor){
  int novoDado;

  while(nColision < MAX_COLISIONS){
    sleep(1);
    novoDado = (random()%100);
    if(novoDado > (30+(5*idTransmissor)) ){
      arrayT[idTransmissor].dado = novoDado;
      arrayT[idTransmissor].status = 1;

      int flagPos = 0;
      int newPos;
      do {
        newPos = (int)(random()% M_SIZE-1);
        flagPos = 0;
        if(arrayPos[newPos] == -1 || arrayPos[newPos] != arrayT[idTransmissor].pos){
          flagPos = 1;
        }
      } while(flagPos==1);

      arrayT[idTransmissor].idDestino = newPos;

      sendData(idTransmissor);
    }
  }
}

main(){

  inicializaMeio();
  view();
  int opcao, i;

  inicializaTransmissores(arrayT);

	pthread_t thread[N_TRANSMISSORES];
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
            for(i=0; i<N_TRANSMISSORES; i++){
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
  Enviar jams
  Backoff/tempo de espera
  Threads
*/
