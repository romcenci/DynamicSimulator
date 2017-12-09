#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "rands.h"

#define alfa 2
#define L 5 // Volume
#define N 200 // Número de particulas
#define R 0.1 // Escala potencial
#define eps 0.0333 // Escala potencial
#define dt 0.001
#define scale 1 // Escala plot tempo

int main(int argc, char **argv){
  int i, j;
  double x[N], tmp, dist, posx;
  double T, t, dw, E;
  
  T=atof(argv[1]); // 0.4
  
  srand(time(0));
  for(i=0; i<N; i++){x[i]=(double)rand()/RAND_MAX*L;}

  for(t=0; t<100; t+=dt){
    E=0;
    for(i=0; i<N; i++){
      x[i]+=sqrt(dt)*sqrt(2*T)*ngaussian();
      for(j=0; j<N; j++){
	if(i!=j){
	  dist=min(min(x[i]-x[j],x[i]-x[j]+L),x[i]-x[j]-L);
	  tmp=eps*exp(-pow(fabs(dist)/R,alfa))*(-alfa*pow(fabs(dist)/R,alfa-1))*(1/R)*dt;
	  x[i]+=(dist>0 ? tmp : -tmp);
	  x[i]=fmod(x[i]+L,L);
	  E+=eps*exp(-pow(fabs(dist)/R,alfa));
	}
      }
      printf("%lf\n",x[i]);
    }
    E/=N*N*4;
    //printf("%lf\n",E);
    t+=dt;
    //////////////////////////////////////////////////////////////////////
  }
  return 0;
}
