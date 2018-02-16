/* Imprime apenas N/5 de N partículas, para facilitar visualização, imprime no formato para usar no DynSimSDL, compilar DynSimSDL com "gcc DynSimSDL.c -lSDL2 -o DynSimSDL" depois roda com "./a.out T | ./DynSim -l N/5" */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "rands.h"

#define L 15 // Volume
#define N 3000 // Número de particulas

#define alfa 2 // Gaussiana
#define R 0.1 // Escala potencial
#define eps 0.0333 // Escala potencial
#define dt 0.001 // 0.001

void update(double *x, double T){
  int i, j;
  double tmp, dist, xnew[N];

  for(i=0; i<N; i++){
    double force = 0;
    
    xnew[i] = x[i] + sqrt(dt)*sqrt(2*T)*ngaussian();
    for(j=0; j<N; j++){
      if(i!=j){
	dist=min(min(x[i]-x[j],x[i]-x[j]+L),x[i]-x[j]-L);
	tmp=eps*exp(-pow(fabs(dist)/R,alfa))*(-alfa*pow(fabs(dist)/R,alfa-1))*(1/R)*dt;
        force+=(dist>0 ? tmp : -tmp);
      }
    }
    xnew[i]+=force;
    xnew[i]=fmod(xnew[i]+L,L);
  }

  for(i=0; i<N; i++)
    x[i]=xnew[i];
}
      
int main(int argc, char **argv){
  int i, k;
  double x[N], T;
  FILE *f1;

  if(argc==2){
    T=atof(argv[1]);
  }
  else{
    printf("Digite no formato ./a.out \"Temperatura\"\n");
    return 1;
  }
    
  srand(time(0));
  for(i=0; i<N; i++){x[i]=(double)rand()/RAND_MAX*L;}

  for(k=0; k<4000; k++){
    update(x, T);

    for(i=0; i<N; i++){
      if(i%5==0){
	printf("%lf\n",x[i]/L);
      }
    }
  }

  return 0;
}
