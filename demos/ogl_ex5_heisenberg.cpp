/*Programa teste com interações dipolar, de troca, com campo externo e anisotropias*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
//#include <fftw3.h>
//******************************************************************************
//******************************************************************************
#define FRANDOM ((double) rand()/RAND_MAX)
//	#define DEBUG
#define PI M_PI
#define tmax 200000
#define ns 100 // corte no tempo
#define Lx 10 // tamanho da rede na direção x, número de partículas no eixo x
#define Ly 10 // tamanho da rede na direção y, número de partículas no eixo y
#define Lz 1	// tamanho da rede na direção z, número de partículas no eixo z
#define co 3
#define dx 1.0
#define dy 1.0
#define dz 1.0
#define dip 10
//******************************************************************************
//**************************Definição das constantes****************************
//******************************************************	************************
double Kb=1; // constante de Boltzmann [J/K]
double mu=1; //1.25664e-06 permeabilidade magnética [T.m/A]
double Ms=1; // magnetização de saturação [A/m]
double V= 1;//6.702e-27; // volume da partícula [m³]
double gama=1; // razão giromagnética [1/Ts}
double Ku= 1.0; // densidade de energia de anisotropia [J/m3]
double Kc= 1.0;
double dt= 5e-3;// passo de tempo [s]
double T = 1 ;	 	; // temperatura [axes]
double alfa =0.2;
double lambda= 1./(1+alfa*alfa);
double A =1;
double lex = 2*A/(mu*V*Ms*Ms);
double Nx,Ny,Nz;
double ntp = Lx*Ly*Lz; // volume da rede - número total de partículas
//-------------------------------------------a-------------------------------
#define direction 1// configuração inicial dos spins: 0 = alinhados, 1 = aleatório
#define dil 0.0
//--------------------------------------------------------------------------
//----------------------acrescentando energias e campos---------------------
//------------------------1 = ligado; 0 = desligado-------------------------
#define C_ani_u 1

//**************************************************************************
//**************************Controle para GNUPLOT***************************
int animacao = 1;
double plot = 2; 	// 0 = 2D, 1 = 3D com esquema de cores, 2 = 3D sem esquema de cores
int snap = 0;		// snapshot da configuração dos spins - 0 = não faz, 1 = faz
//**************************************************************************
#include"ogl_ex5_heisenberg_fcn.h"
//**************************************************************************
//**********************************função principal************************
clock_t start, stop;

int main(){
long int t;
long double tempo = 0;
int i=0,j=0,k=0,hc=0,hcc=0,L=0,M=0,N=0, I=0,J=0,K=0,a=0,b=0,c=0,ii,jj,kk;        //vizinhos
int vxd=0,vxe=0,vyd=0,vye=0,vzd=0,vze=0,part=0;

double ****mag={0},****eax={0},****d={0},****hex={0},****dw={0},****rr={0};
double ***Kxx={0}, ***Kxy={0}, ***Kxz={0}, ***Kyy={0}, ***Kyz={0},***Kzz={0};
double ****H_anis_u={0},****H_anis_c={0},****H_ef={0},****H_troca={0}, ****H_ext={0},****H_dip={0};
double E_anis_u = 0,E_anis_c = 0,E_cex = 0,E_dip = 0,E_exc = 0, Energia_total =0;
double Ea=0,P=0,mod_m=0,mx=0,my=0,mz=0, mxm=0,mym=0,mzm=0,mzt=1,mxt=1,myt=1,mm=0,r=0 ;
double ang_mhex=0,ang_hhex=0,ang_meax=0,mod_h=0,ene=0,Z,D,soene=0,ang_mh=0;
int l,m,n;
double mod_H_dip = 0,mod_H_troca = 0,Dist;
//******************************************************************************
//******************ALOCAÇÃO DINÂMICA DE MEMÓRIA********************************
//**************************alocando memória************************************
mag      = (double****)calloc(co,sizeof(double***)*co);
eax      = (double****)calloc(co,sizeof(double***)*co);
d        = (double****)calloc(co,sizeof(double***)*co);
hex      = (double****)calloc(co,sizeof(double***)*co);
H_anis_u = (double****)calloc(co,sizeof(double***)*co);
H_anis_c = (double****)calloc(co,sizeof(double***)*co);
H_ef     = (double****)calloc(co,sizeof(double***)*co);
H_troca  = (double****)calloc(co,sizeof(double***)*co);
H_ext    = (double****)calloc(co,sizeof(double***)*co);
H_dip    = (double****)calloc(co,sizeof(double***)*co);
dw       = (double****)calloc(co,sizeof(double***)*co);
for(hc = 0; hc < co; hc++ ){
mag[hc]      = (double***)calloc(Lx,sizeof(double**)*Lx);
eax[hc]      = (double***)calloc(Lx,sizeof(double**)*Lx);
d[hc] 	    = (double***)calloc(Lx,sizeof(double**)*Lx);
hex[hc] 	  	 = (double***)calloc(Lx,sizeof(double**)*Lx);
H_anis_u[hc] = (double***)calloc(Lx,sizeof(double**)*Lx);
H_anis_c[hc] = (double***)calloc(Lx,sizeof(double**)*Lx);
H_ef[hc] 	 = (double***)calloc(Lx,sizeof(double**)*Lx);
H_troca[hc]  = (double***)calloc(Lx,sizeof(double**)*Lx);
H_ext[hc]    = (double***)calloc(Lx,sizeof(double**)*Lx);
dw[hc]		 = (double***)calloc(Lx,sizeof(double**)*Lx);
H_dip[hc]	 = (double***)calloc(Lx+1,sizeof(double**)*(Lx+1));
for(i = 0; i < Lx; i++ ){
mag[hc][i]      = (double**)calloc(Ly,sizeof(double*)*Ly);
eax[hc][i]      = (double**)calloc(Ly,sizeof(double*)*Ly);
d[hc][i] 	    = (double**)calloc(Ly,sizeof(double*)*Ly);
hex[hc][i] 		 = (double**)calloc(Ly,sizeof(double*)*Ly);
H_anis_u[hc][i] = (double**)calloc(Ly,sizeof(double*)*Ly);
H_anis_c[hc][i] = (double**)calloc(Ly,sizeof(double*)*Ly);
H_ef[hc][i] 	 = (double**)calloc(Ly,sizeof(double*)*Ly);
H_troca[hc][i]  = (double**)calloc(Ly,sizeof(double*)*Ly);
H_ext[hc][i]    = (double**)calloc(Ly,sizeof(double*)*Ly);
dw[hc][i] 	    = (double**)calloc(Ly,sizeof(double*)*Ly);
H_dip[hc][i] 	 = (double**)calloc(Ly+1,sizeof(double*)*(Ly+1));
for(j = 0; j < Ly; j++ ){
mag[hc][i][j]     = (double*)calloc(Lz,sizeof(double)*Lz);
eax[hc][i][j]     = (double*)calloc(Lz,sizeof(double)*Lz);
d[hc][i][j] 	   = (double*)calloc(Lz,sizeof(double)*Lz);
hex[hc][i][j] 		= (double*)calloc(Lz,sizeof(double)*Lz);
H_anis_u[hc][i][j]= (double*)calloc(Lz,sizeof(double)*Lz);
H_anis_c[hc][i][j]= (double*)calloc(Lz,sizeof(double)*Lz);
H_ef[hc][i][j] 	= (double*)calloc(Lz,sizeof(double)*Lz);
H_troca[hc][i][j] = (double*)calloc(Lz,sizeof(double)*Lz);
H_ext[hc][i][j]   = (double*)calloc(Lz,sizeof(double)*Lz);
dw[hc][i][j] 	   = (double*)calloc(Lz,sizeof(double)*Lz);
H_dip[hc][i][j]   = (double*)calloc(Lz+1,sizeof(double)*(Lz+1));
}}}
Kxx  = (double***)calloc(Lx,sizeof(double**)*Lx);
Kxy  = (double***)calloc(Lx,sizeof(double**)*Lx);
Kxz  = (double***)calloc(Lx,sizeof(double**)*Lx);
Kyy  = (double***)calloc(Lx,sizeof(double**)*Lx);
Kyz	 = (double***)calloc(Lx,sizeof(double**)*Lx);
Kzz  = (double***)calloc(Lx,sizeof(double**)*Lx);
for(L = 0; L < Lx; L++ ){
Kxx[L]   = (double**)calloc(Ly,sizeof(double*)*Ly);
Kxy[L]   = (double**)calloc(Ly,sizeof(double*)*Ly);
Kxz[L]   = (double**)calloc(Ly,sizeof(double*)*Ly);
Kyy[L]   = (double**)calloc(Ly,sizeof(double*)*Ly);
Kyz[L]	= (double**)calloc(Ly,sizeof(double*)*Ly);
Kzz[L]	= (double**)calloc(Ly,sizeof(double*)*Ly);
for(M = 0; M < Ly; M++ ){
Kxx[L][M] = (double*)calloc(Lz,sizeof(double)*Lz);
Kxy[L][M] = (double*)calloc(Lz,sizeof(double)*Lz);
Kxz[L][M] = (double*)calloc(Lz,sizeof(double)*Lz);
Kyy[L][M] = (double*)calloc(Lz,sizeof(double)*Lz);
Kyz[L][M] = (double*)calloc(Lz,sizeof(double)*Lz);
Kzz[L][M] = (double*)calloc(Lz,sizeof(double)*Lz);
}}

rr  = (double****)calloc(co,sizeof(double***)*co);
for(hc = 0; hc < co; hc++ ){
rr[hc] = (double***)calloc(Lx+1,sizeof(double**)*(Lx+1));
for(i = 0; i < Lx+1; i++ ){
rr[hc][i] = (double**)calloc(Ly+1,sizeof(double*)*(Ly+1));
for(j = 0; j < Ly+1; j++ ){
rr[hc][i][j] = (double*)calloc(Lz+1,sizeof(double)*(Lz+1));
}}}

//*************************memória alocada**************************************
//******************************************************************************
//*************************abrindo arquivos para serem gravados*****************
FILE *f1,*f2,*f3,*f4;
//******************************************************************************
//******************************************************************************
#ifndef DEBUG
srand(time(0));
#endif
//******************************************************************************
//*******************orientação inicial da magnetização*************************
//******************************************************************************
configuracao(direction,Lx,Ly,Lz,mag);
//..............................................................................
//...............................EIXO FÁCIL.....................................
for(i=0; i<Lx; i++){
  for(j=0; j<Ly ; j++){
    for(k=0; k<Lz ; k++){
      eax[0][i][j][k] = 0.0;
      eax[1][i][j][k] = 0.0;
      eax[2][i][j][k] = 0.0;
      //..............................................................................
      //.............................CAMPOS EXTERNO...................................
      hex[0][i][j][k] = 0.0;
      hex[1][i][j][k] = 0.0;
      hex[2][i][j][k] = -1.0;
    } } }
    //..............................................................................
    //...............................INTENSIDADE DO RUÍDO...........................
    D = (alfa*Kb*T)/(V*Ms*mu);

    for(i=0; i<Lx; i++){
      for(j=0; j<Ly; j++){
        for(k=0; k<Lz; k++){
          rr[0][i][j][k] = i*dx;
          rr[1][i][j][k] = j*dy;
          rr[2][i][j][k] = k*dz;
        }}}

        for(t=0; t<tmax; t++){

          for(hc=0; hc<co; hc++){
            for(i=0; i<Lx; i++){
              for(j=0; j<Ly; j++){
                for(k=0; k<Lz; k++){
                  H_ef[hc][i][j][k]= 0;
                  H_anis_u[hc][i][j][k] = 0;
                  mxt = 0;
                  myt = 0;
                  mzt = 0;
                  mm = 0;
		  } } } }

      for(i=0; i<Lx; i++){
        for(j=0; j<Ly; j++){
          for(k=0; k<Lz; k++){


            if(T != 0 || alfa != 0)
            for(hc=0; hc<co; hc++)
            dw[hc][i][j][k] = ngaussian()*sqrt(2*(alfa*T*dt)/(1+alfa*alfa));
            else
            for(hc=0; hc<co; hc++)
            dw[hc][i][j][k] = 0;


            if(C_ani_u == 1){
              H_anis_u[0][i][j][k] = 0;
              H_anis_u[1][i][j][k] = 0;
              H_anis_u[2][i][j][k] =  (2*Ku/(mu*Ms*Ms))*mag[2][i][j][k];
            }
            else
            for(hc=0; hc<co; hc++)
            H_anis_u[hc][i][j][k] = 0;

            for(hc=0; hc<co; hc++)
            H_ef[hc][i][j][k] +=  H_anis_u[hc][i][j][k];

            //..............................................................................
            //.............................MÉDIA DA MAGNETIZAÇÃO NO TEMPO...................
            if(t%ns != 0){
              mx += mag[0][i][j][k];
              my += mag[1][i][j][k];
              mz += mag[2][i][j][k];
            }
            if(t%ns == 0){
              mxm = mx/t;
              mym = my/t;
              mzm = mz/t;
              //..............................................................................
              //..............................................................................
              tempo = (double) t*dt;

              mxt = mag[0][i][j][k];
              myt = mag[1][i][j][k];
              mzt = mag[2][i][j][k];

              mm += sqrt(pow(mxt/(Lx*Ly*Lz),2) + pow(myt/(Lx*Ly*Lz),2) + pow(mzt/(Lx*Ly*Lz),2));

              mx = 0;
              my = 0;
              mz = 0;
            }
          } } }

          // anima(animacao,plot,Lx,Ly,Lz,mag); // descomentar pra ser como o marcelo fez
          int iii, jjj; //vamos assumir um sistema planar
          double tamanho_vec;
          for(iii = 0; iii < Lx; iii++){
            for(jjj = 0; jjj < Ly; jjj++){
            tamanho_vec = sqrt(pow(mag[0][iii][jjj][0],2)+pow(mag[1][iii][jjj][0],2)+pow(mag[2][iii][jjj][0],2));
            // tamanho_vec = 1.0;
            printf("%f\t%f\t%f\n", mag[0][iii][jjj][0]/tamanho_vec,mag[1][iii][jjj][0]/tamanho_vec
            ,mag[2][iii][jjj][0]/tamanho_vec);
          }
          }

          update(mag,H_ef,dw); //Faz as atualizações da simulação
          //..............................................................................
        }

        stop = clock();
        printf("t = %ld --- Time =%f min\n",t,(float)(stop-start)/(CLOCKS_PER_SEC)/60.);

        fclose(f1);

        //******************************************************************************
        //******************ALOCAÇÃO DINÂMICA DE MEMÓRIA********************************
        //**************************liberando memória***********************************

        for(i = 0; i < co; i++ ){
          for(j = 0; j < Lx; j++ ){
            for(k = 0; k < Ly; k++ ){

              free(mag[i][j][k]);
              free(eax[i][j][k]);
              free(dw[i][j][k]);
              free(d[i][j][k]);
              free(hex[i][j][k]);
              free(H_anis_u[i][j][k]);
              free(H_anis_c[i][j][k]);
              free(H_ef[i][j][k]);
              free(H_troca[i][j][k]);
              free(H_ext[i][j][k]);
              free(H_dip[i][j][k]);
            }

            free(mag[i][j]);
            free(eax[i][j]);
            free(dw[i][j]);
            free(d[i][j]);
            free(hex[i][j]);
            free(H_anis_u[i][j]);
            free(H_anis_c[i][j]);
            free(H_ef[i][j]);
            free(H_troca[i][j]);
            free(H_ext[i][j]);
            free(H_dip[i][j]);
          }
          free(mag[i]);
          free(eax[i]);
          free(dw[i]);
          free(d[i]);
          free(hex[i]);
          free(H_anis_u[i]);
          free(H_anis_c[i]);
          free(H_ef[i]);
          free(H_troca[i]);
          free(H_ext[i]);
          free(H_dip[i]);
        }
        free(mag);
        free(eax);
        free(dw);
        free(d);
        free(hex);
        free(H_anis_u);
        free(H_anis_c);
        free(H_ef);
        free(H_troca);
        free(H_ext);
        free(H_dip);


        for(M = 0; M < Lx; M++ ){
  for(N = 0; N < Ly; N++ ){

    free(Kxx[M][N]);
    free(Kxy[M][N]);
	 free(Kxz[M][N]);
	 free(Kyy[M][N]);
	 free(Kyz[M][N]);
	 free(Kzz[M][N]);
  }

  free(Kxx[M]);
  free(Kxy[M]);
  free(Kxz[M]);
  free(Kyy[M]);
  free(Kyz[M]);
  free(Kzz[M]);
}

free(Kxx);
free(Kxy);
free(Kxz);
free(Kyy);
free(Kyz);
free(Kzz);


for(i = 0; i < co; i++ ){
for(j = 0; j < Lx; j++ ){
for(k = 0; k < Ly; k++ ){
free(rr[i][j][k]);

}
free(rr[i][j]);

}
free(rr[i]);

}
free(rr);

//************************memória liberada**************************************
//******************ALOCAÇÃO DINÂMICA DE MEMÓRIA********************************
//******************************************************************************
} // FIM DO MAIN(), FIM DA SIMULAÇÃO
