#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fftw3.h>
#include <time.h>


#define alpha (0.23f)
#define waveq 0.8f
#define timeprinter 600
#define maxwindows 1000000
#define num_cel 10
#define M 800
#define N 1
#define L  (M*N)
#define X (L/sqrt(2)/2/num_cel)
#define dx M_PI/X
// #define dx 0.088


#define dt 0.01f

#define tmax timeprinter*maxwindows
#define tprint 100

#define pi M_PI
#define k0 (pi/M)
#define Lk  (L/2+1)



double *fi3,*ftmp,*F,*I;
fftw_complex *fi3k,*fik,*ftmpk;
fftw_plan f3r2c,f3c2r,fr2c,fc2r;

double *fnx;
fftw_complex *fnxk;
fftw_plan fnxr2c,fnxc2r;

clock_t start, stop;
double real_time,cpu_time;
double k2[Lk],k[Lk];

void vetores(double *fi)
{
  int i,j,ii;
  double tmp;

  cpu_time = clock();
  real_time = time(NULL);


  F = fftw_malloc(sizeof(double)*(Lk));
  I = fftw_malloc(sizeof(double)*(Lk));

  fik = fftw_malloc(sizeof(fftw_complex)*(Lk));

  fi3 = fftw_malloc(sizeof(double)*L);
  fi3k = fftw_malloc(sizeof(fftw_complex)*(Lk));

  ftmp = fftw_malloc(sizeof(double)*L);
  ftmpk = fftw_malloc(sizeof(fftw_complex)*(Lk));

  fr2c = fftw_plan_dft_r2c_1d(L,fi,fik,FFTW_PATIENT);
  fc2r = fftw_plan_dft_c2r_1d(L,fik,fi,FFTW_PATIENT);

  f3r2c = fftw_plan_dft_r2c_1d(L,fi3,fi3k,FFTW_PATIENT);
  f3c2r = fftw_plan_dft_c2r_1d(L,fi3k,fi3,FFTW_PATIENT);

  fnx = fftw_malloc(sizeof(double)*L);
  fnxk = fftw_malloc(sizeof(fftw_complex)*(Lk));
  fnxr2c = fftw_plan_dft_r2c_1d(L,fi3,fi3k,FFTW_PATIENT);
  fnxc2r = fftw_plan_dft_c2r_1d(L,fi3k,fi3,FFTW_PATIENT);

  for(i=0;i<L/2+1;i++){
    k[i]=(i)*2.0*pi/(L*dx);
    k2[i]= powf(k[i],2);
  }

  for(i=0;i<Lk;i++){
    /* IMPLICITO */

    F[i] = 1.0f/(1 + k2[i]*k2[i]*dt + alpha*dt - k2[i]*dt)/(double) L;
    I[i] = k[i]/2.0*dt*F[i];
  }

  /* for(i=0;i<Lk;i++) */
  /*   printf("%f %f %f\n",k[i],I[i],F[i]); */


  return;
}

void update(double *fi)
{
  int i;

  // diretor(fi);

  for(i=0;i<L;i++)
  fi3[i] = fi[i]*fi[i];
    //fi3[i]= dt*(fnx[i]*fnx[i])/dx2;

  /* for(i=0;i<L;i++) */
  /*   printf("%d %.6e %.6e\n",i,fi3[i],fnx[i]); */

  /*transforma fourier*/
  fftw_execute(fr2c);   fftw_execute(f3r2c);

  /*atualiza:*/
  for(i=0;i<Lk;i++){
    fik[i][0] =  F[i]*fik[i][0] +  fi3k[i][1]*I[i];
    fik[i][1] =  F[i]*fik[i][1] -  fi3k[i][0]*I[i];
  }

  /*anti-transforma*/
  fftw_execute(fc2r);

  return;
}
void espectro_potencias(double *fi, double *ck){
  int i,j;

  for(i=0;i<L;i++)
    fi3[i] = fi[i];

  fftw_execute(f3r2c);

  for(i=0;i<Lk;i++)
    ck[i] =  (pow(fi3k[i][0],2) + pow(fi3k[i][1],2))/L;

  return;
}

double max(double *fi,int tamanho){

  double xmax=0.0;
  int i;

  for(i=0;i<tamanho;i++)
    xmax = (fi[i]>xmax?fi[i]:xmax);

  return xmax;
}

void delay(double number_of_seconds)
{
    // Converting time into milli_seconds
    double milli_seconds = 1000 * number_of_seconds;

    // Stroing start time
    clock_t start_time = clock();

    // looping till required time is not acheived
    while (clock() < start_time + milli_seconds)
        ;
}
int main(void)
{
  double *fi,e,cc,K[5],g[2], somaU = 0, somaU2 = 0;
  int i,j;
  FILE *f1;
  double spectro[Lk];
  char nome[100];

  fi = fftw_malloc(sizeof(double)*L);
  vetores(fi);

  srand (time(NULL));

  for(i=0;i<L;i++)
  // fi[i] = sin(num_cel*2*M_PI*i/((double) L)) ;
    fi[i] = 2.0*rand()/RAND_MAX - 1.0;
    //  fi[i] = sin(waveq*i/((double) L));
    //Condição inicial para as drifting
  start = clock();


  for(j=0;j<tmax;j++){
    somaU = somaU2 = 0;
    // Printa <U> e <U**2>
      // for(i = 0; i<L;i++){
      //   somaU += fi[i];
      //   somaU2 += fi3[i];
      // }
      // printf("%d %f %f\n",j, somaU/(double) L, sqrt(somaU2/(double) L));

    for(i=0;i<tprint;i++)
      update(fi);
    //
    espectro_potencias(fi, spectro);
    for(i=0;i<Lk;i++)
      spectro[i] = log(spectro[i]);

     e = max(spectro,Lk);
     for(i=0;i<Lk;i++){
       printf("%d\t %f\n",i,spectro[i]/(4*e));
     }
     //printf("#draw\n");

  }
//  e = max(fi);
  // for(i=0;i<L;i++)
    // printf("%d %d %f \n",i,j%timeprinter,fi[i]);
  // printf("#draw\n");

  stop = clock();

  return 0;
}
