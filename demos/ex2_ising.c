/***************************************************************/
/* 1Ising2d-Simulação.c					       */
/* 							       */
/* Realiza a simulação do modelo de Ising 2d, medindo a	       */
/* evolução da energia e magnetização, ou anima a simulação    */
/* da evolução da rede a partir do "| gnuplot".		       */
/* 							       */
/* Primeiros passos, na função init() escolhe se quer	       */
/* começar frio (tudo alinhado) ou quente (1/2 a 1/2),	       */
/* função update() atualiza N sítios da rede escolhidos	       */
/* aleatoriamente, funções measEner() measMag() medem	       */
/* energia e magnetizaão respectivamente, função plot() plota. */
/***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define L 256
#define N L*L
#define XNN 1
#define YNN L

#define MC 100

int s[N];
double prob[5];
double beta;

void init(){
  int i;
  srand(time(0));

  for(i=2; i<5; i+=2) prob[i]=exp(-2.*beta*(double)i); // Calcula as probabilidades possíveis

  // Começo FRIO:
  /* for(i=0; i<N; i++) s[i]=1; */

  // Começo QUENTE:
    for(i=0; i<N; i++){
      if(rand()%2 == 0) s[i]=1;
      else s[i]=-1;
    }
  }

  void update(){
    int i, j;
    int nn, snn, dE;

    for(j=0; j<N; j++){
    i=(double)rand()/RAND_MAX*N; // Escolhe alguém na rede

    if((nn=i+XNN)>=N) nn-=N; snn =s[nn]; // Condições de contorno + Soma vizinhos
    if((nn=i-XNN)< 0) nn+=N; snn+=s[nn];
    if((nn=i+YNN)>=N) nn-=N; snn+=s[nn];
    if((nn=i-YNN)< 0) nn+=N; snn+=s[nn];
    dE=snn*s[i]; // (Ei-Ej)/2

    // Condições para flipar
    if(dE <= 0) s[i]=-s[i];
    else if((double)rand()/RAND_MAX < prob[dE]) s[i]=-s[i];
  }
}

double measEner(){
  double e=0;
  int nn, snn, i;

  for(i=0; i<N; i++){
    // Soma sobre os necessários, i-XNN e i-YNN são redundantes
    if((nn=i+XNN)>=N) nn-=N; snn =s[nn];
    if((nn=i+YNN)>=N) nn-=N; snn+=s[nn];
    e+=snn*s[i];
  }

  return -e/N;
}

double measMag(){
  double m=0;
  int i;

  for(i=0; i<N; i++) m+=s[i];

    return m;
}

void plot(){
  int i;

  //printf("p '-' u 1:2:3 w p pt 5 ps 0.7 pal\n");
  for(i=0; i<N; i++){
    printf("%d ", s[i]);
  }
  //printf("e\n");
}

int main(int argc, char *argv[]){
  int mc;
  beta=2;
  init();

  for(mc=0; mc<MC; mc++){
    unsigned char comm = getc(stdin);
    if(comm != '\n')
    {
      fprintf(stderr,"invalid command: 0x%X\n",comm);
      break;
    }

    update();

    if(mc%1 == 0){
      //printf("%d %lf %lf\n", mc, measEner(), measMag());
      plot();
    }
    printf("\n");
    fflush(stdout);
  }

  return 0;
}
