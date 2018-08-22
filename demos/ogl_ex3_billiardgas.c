// 0.157*raio = ps

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define raio 40
#define N 50
#define dt 0.1

#define width 1000
#define height 1000

#define xRandom (double)rand()/RAND_MAX*(width-raio)
#define yRandom (double)rand()/RAND_MAX*(height-raio)
#define velRandom (double)rand()/RAND_MAX > 0.5 ? (double)rand()/RAND_MAX*10 : -(double)rand()/RAND_MAX*10

int i, j;
double x[N];
double y[N];
double vx[N];
double vy[N];

double dvx, dvy, dx, dy, vtemp;
double th;

int flag[N][N];
int flagBound[N];
int flg[N];

void init(){
  for (i=0; i<N; i++) {
    for (j=0; j<N; j++) {
      flag[i][j]=0;
    }
    flagBound[i]=0;
  }

  for (i=0; i<N; i++) {
    while(flg[i]<N-1){
      x[i]=(double)rand()/RAND_MAX*(width-raio);
      y[i]=(double)rand()/RAND_MAX*(height-raio);
      flg[i]=0;
      for (j=0; j<N; j++) {
	if (sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j])) > 2.5*raio) {
	  flg[i]++;
	}
      }
    }
    
    /* vx[i]=2; */
    /* vy[i]=2; */
    vx[i]=velRandom;
    vy[i]=velRandom;
  }
}

void update(){
  for (i=0; i<N; i++) {
    x[i]+=vx[i]*dt;
    y[i]+=vy[i]*dt;
  }
}

void rotate(int k, double th) {
  double ux, uy;

  ux=vx[k];
  uy=vy[k];

  vx[k]=cos(th)*ux+sin(th)*uy;
  vy[k]=-sin(th)*ux+cos(th)*uy;
}

void boundCollision(){
  for (i=0; i<N; i++) { // Restaura flag de parede
    if (flagBound[i]==1) {
      if (x[i]>0 && x[i]<width && y[i]>0 && y[i]<height) {
        flagBound[i]=0;
      }
    }
  }
  for (i=0; i<N; i++) { // Boundary
    if (flagBound[i]==0) {
      if (x[i]+raio >= width || x[i]-raio <= 0) {
        vx[i]=-vx[i];
        flagBound[i]=1;
      }
      if (y[i]+raio >= height || y[i]-raio <= 0) {
        vy[i]=-vy[i];
        flagBound[i]=1;
      }
    }
  }
}

void collision(){
  for (i=0; i<N; i++) { // Restaura flag de colisão x1
    for (j=0; j<N; j++) {
      if (flag[i][j]==1) {
        if (sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j])) > 2*raio) {
          flag[i][j]=0;
        }
      }
    }
  }
  for (i=0; i<N; i++) { // Colisão
    for (j=0; j<i; j++) {
      if (flag[i][j]==0) {
        if (sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j])) < 2*raio) {
          dvx = vx[i]-vx[j];
          dvy = vy[i]-vy[j];

          dx = x[i]-x[j];
          dy = y[i]-y[j];

          th = atan2(dy, dx);
          rotate(i, th);
          rotate(j, th);

          vtemp=vx[j];
          vx[j]=vx[i];
          vx[i]=vtemp;

          rotate(i, -th);
          rotate(j, -th);

          flag[i][j]=1;
        }
      }
    }
  }
}

void plot(){  
  for (i=0; i<N; i++) {
    printf("%lf %lf %lf\n", x[i]/width, y[i]/height, (double)raio);
  }
}

int main(int argc, char *argv[]){
  double t;

  srand(time(0));
  
  init();
  
  for(t=0; t<10000; t+=dt){
    update();

    boundCollision();
    collision();

    plot();
  }
  
  return 0;
}
