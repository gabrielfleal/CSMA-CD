#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<time.h>

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
  int nBackoff;
};
typedef struct Transmissor Transmissor;

Transmissor arrayT[N_TRANSMISSORES];

void inicializaTransmissores(){
  int i, j;
  for (i = 0; i < N_TRANSMISSORES; i++) {
    arrayT[i].id = i;
  	arrayT[i].dado = -1;
    arrayT[i].status = 0;
    arrayT[i].idDestino = -1;
    arrayT[i].dadoRecebido = -1;
    arrayT[i].nBackoff = 0;

    int flagPos = 0;
    int newPos;
    do {
      newPos = (int)(random()% M_SIZE-1);
      flagPos = 0;
      if(arrayPos[newPos]!= -1){
        flagPos = 1;
      }
    } while(flagPos==1);

    arrayT[i].pos = newPos;
    arrayPos[arrayT[i].pos] = arrayT[i].id;
  }
}

void inicializaMeio(){
  int i;
  for (i = 0; i < M_SIZE; i++) {
    meio[i] = -1; // -1 significa que o meio está vazio; -2 indica colisão;
    arrayPos[i] = -1; // -1 significa que não há nenhum transmissor naquela posição
  }
}

void view(){
  int i;
  printf("\n --- Meio de transmissão --- \n");
  printf("\nPosição:  ");
  for(i = 0; i < M_SIZE; i++){
    printf("%d  ", i);
  }
  printf("\n\n          ");

  for (i = 0; i < M_SIZE; i++){
    printf("%d ", meio[i]);
  }
  printf("\n\n          ");

  for (i = 0; i < M_SIZE; i++){
    if (arrayPos[i] != -1) {
      printf("%d ", arrayPos[i]);
    }else{
      printf("-1 ");
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

void enviarJam(int pos, int idTransmissor){
  int i;
  for(i = 0; i < M_SIZE; i++){
    int r = M_SIZE - pos + i;
    int l = pos - i;

    if (l >= 0) {
      meio[l] = -2;
    }
    if (r < M_SIZE) {
      meio[r] = -2;
    }

    view();
    fflush(stdout);
    sleep(1);
  }
  sleep(3);
}

void backoff(int idTransmissor){
  if(arrayT[idTransmissor].nBackoff < 16){
    sleep(arrayT[idTransmissor].nBackoff);
  }else{
    arrayT[idTransmissor].nBackoff = 0;
    arrayT[idTransmissor].dado = -1;
    arrayT[idTransmissor].idDestino = -1;
    arrayT[idTransmissor].status = 0;
  }
  limparMeio(idTransmissor);
}

int detectarColisao(int pos, int idTransmissor){
  int flagColisao = 0;
  if(meio[pos] != -1 && meio[pos] != idTransmissor){
    flagColisao = 1;
    printf("\n----- Colisão na posição: %d\n", pos);
    enviarJam(pos, idTransmissor);
  }
  return flagColisao;
}

void enviarDado(int idTransmissor){
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
    sleep(3); //Tempo fixo para verificar se o meio está ocupado
    flagSensing = 0;
    for(i = 0; i < sizeof(M_SIZE); i++){
      if(meio[i] != -1){ //O meio está ocupado
        flagSensing = 1;
        if(i == arrayT[idTransmissor].pos){
          if(meio[i] == -2){ //O sinal -2 indica que ocorreu colision e o jam chegou na posição deste transmissor
            backoff(idTransmissor);
            break;
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

void *geraDado(void *idTransmissor){
  int novoDado;
  int id = *(int*) idTransmissor;
  while(nColision < MAX_COLISIONS){
    sleep(1);
    novoDado = (random()%100);
    if(novoDado > (30+(5*id)) ){
      arrayT[id].dado = novoDado;
      arrayT[id].status = 1;

      int flagPos = 0;
      int newPos;
      do {
        newPos = (int)(random()% M_SIZE-1);
        flagPos = 0;
        if(arrayPos[newPos] == -1 || arrayPos[newPos] != arrayT[id].pos){
          flagPos = 1;
        }
      } while(flagPos==1);

      arrayT[id].idDestino = newPos;
      enviarDado(id);
    }
  }
}

main(){
  srand(time(0));
  int opcao, i;
  inicializaMeio();
  inicializaTransmissores();
  view();

  int nArray[N_TRANSMISSORES];
  pthread_t thread[N_TRANSMISSORES];
  void *thread_result;
  do{
  	printf("Escolha uma das opções abaixo:\n1 - Iniciar simulação;\n0 - Encerrar;\n");
  	scanf("%d", &opcao);
		switch(opcao){
      case 1:
          for(i = 0; i < N_TRANSMISSORES; i++){
              nArray[i] = i;
              pthread_create(&thread[i], NULL, geraDado, &nArray[i]);
          }
          for(i = 0; i < N_TRANSMISSORES; i++)
              pthread_join(thread[i], &thread_result);
      case 0:
          printf("\nFim da execução.\n");
          break;
      default:
          printf("Erro! Digite novamente");
      }
  } while(opcao!=0);
}
