//**************************************************************************************************************
//**************************************************************************************************************
//**************************************************************************************************************
double ngaussian(void)
  {
  static int iset=0;
  static double gset;
  double fac,r,v1,v2;
  double mu = 0.0;
  double s = 1.0;
  if (iset==0) {
  do {
  v1=2.0*FRANDOM-1.0;
  v2=2.0*FRANDOM-1.0;
  r=v1*v1+v2*v2;
  }
  while (r>=1 || r==0.0);
  fac=sqrt(-s*log(r)/r);
  gset=mu + v1*fac;
  iset=1;
  return mu + v2*fac;
  }
  else {
  iset=0;
  return gset;
  }
  }
//**************************************************************************************************************
//**************************************************************************************************************
//**************************************************************************************************************
double prodvet(double ****v1, double ****v2,double ****result)
{
  int ii,jj,hc,i,j,k;

//printf("*****PRODVET*****\n");


  for(i=0;i<Lx;i++){
  for(j=0;j<Ly;j++){
  for(k=0;k<Lz;k++){
  result[0][i][j][k] = v1[1][i][j][k]*v2[2][i][j][k] - v1[2][i][j][k]*v2[1][i][j][k];
  result[1][i][j][k] = v1[2][i][j][k]*v2[0][i][j][k] - v1[0][i][j][k]*v2[2][i][j][k];
  result[2][i][j][k] = v1[0][i][j][k]*v2[1][i][j][k] - v1[1][i][j][k]*v2[0][i][j][k];
  }}}
}
//**************************************************************************************************************
//**************************************************************************************************************
//**************************************************************************************************************
double dotpro(double ****v, double ****u)
{
  double result = 0.0;
  int hc,i,j,k;
//  printf("*****DOTPRO*****\n");
  for(hc=0;hc<3;hc++)
  for(i=0;i<Lx;i++)
  for(j=0;j<Ly;j++)
  for(k=0;k<Lz;k++)
  result += v[hc][i][j][k]*u[hc][i][j][k];

  return result;

}
//**************************************************************************************************************
//**************************************************************************************************************
//**************************************************************************************************************
double direcao(double ****v, double ****u)
{
  double angulo = 0.0;
  int hc,i,j,k;
  double acos(double x);

  angulo = acos(dotpro(v,u)/(sqrt(dotpro(v,v))*sqrt(dotpro(u,u))))*(180/PI);
  return angulo;
}
//**************************************************************************************************************
//**************************************************************************************************************
//**************************************************************************************************************
double modulo(double ****a)
{
  double result = 0.0;
  int hc,i,j,k;

  result = sqrt(dotpro(a,a));
  return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
//**************************************************************************************************************
void f(double ****m, double ****h, double ****result)
{
  double ****mh, ****mmh;
  int hc,i,j,k;

  mh      				= (double****)calloc(co,sizeof(double***)*co);
  mmh     				= (double****)calloc(co,sizeof(double***)*co);
  for(hc = 0; hc < co; hc++ ){
	 mh[hc]      			= (double***)calloc(Lx,sizeof(double**)*Lx);
	 mmh[hc]      		= (double***)calloc(Lx,sizeof(double**)*Lx);
  for(i = 0; i < Lx; i++ ){
  mh[hc][i]      		= (double**)calloc(Ly,sizeof(double*)*Ly);
  mmh[hc][i]      	= (double**)calloc(Ly,sizeof(double*)*Ly);
  for(j = 0; j < Ly; j++ ){
  mh[hc][i][j]      	= (double*)calloc(Lz,sizeof(double)*Lz);
  mmh[hc][i][j]      = (double*)calloc(Lz,sizeof(double)*Lz);
  }}}

  prodvet(m,h,mh);
  prodvet(m,mh,mmh);

  for(hc=0;hc<co;hc++)
  for(i=0;i<Lx;i++)
  for(j=0;j<Ly;j++)
  for(k=0;k<Lz;k++)
	 result[hc][i][j][k] = -(mh[hc][i][j][k] + (alfa)*mmh[hc][i][j][k]);

  for(i = 0; i < co; i++ ){
	 for(j = 0; j < Lx; j++ ){
		for(k = 0; k < Ly; k++ ){

		  free(mh[i][j][k]);
		  free(mmh[i][j][k]);
		}
		free(mh[i][j]);
		free(mmh[i][j]);
	 }
	 free(mh[i]);
	 free(mmh[i]);
  }
  free(mh);
  free(mmh);

  return;
}
//**************************************************************************************************************
//**************************************************************************************************************
//**************************************************************************************************************
void g(double ****m, double ****d,double ****result)
{
  double ****md, ****mdd;
  int hc,i,j,k;

  md      		  = (double****)calloc(co,sizeof(double***)*co);
  mdd     		  = (double****)calloc(co,sizeof(double***)*co);
  for(hc = 0; hc < co; hc++ ){
  md[hc]      	  = (double***)calloc(Lx,sizeof(double**)*Lx);
  mdd[hc]        = (double***)calloc(Lx,sizeof(double**)*Lx);
  for(i = 0; i < Lx; i++ ){
  md[hc][i]      = (double**)calloc(Ly,sizeof(double*)*Ly);
  mdd[hc][i]      = (double**)calloc(Ly,sizeof(double*)*Ly);
  for(j = 0; j < Ly; j++ ){
  md[hc][i][j]    = (double*)calloc(Lz,sizeof(double)*Lz);
  mdd[hc][i][j]   = (double*)calloc(Lz,sizeof(double)*Lz);
  }}}

  prodvet(m,d,md);
  prodvet(m,md,mdd);

  for(hc=0;hc<co;hc++)
  for(i=0;i<Lx;i++)
  for(j=0;j<Ly;j++)
  for(k=0;k<Lz;k++)
	 result[hc][i][j][k] = -(md[hc][i][j][k] + (alfa)*mdd[hc][i][j][k]);

  for(i = 0; i < co; i++ ){
	 for(j = 0; j < Lx; j++ ){
		for(k = 0; k < Ly; k++ ){
		  free(md[i][j][k]);
		  free(mdd[i][j][k]);
		}
		free(md[i][j]);
		free(mdd[i][j]);
	 }
	 free(md[i]);
	 free(mdd[i]);
  }
  free(md);
  free(mdd);

  return;
}
//**************************************************************************************************************
//**************************************************************************************************************
//**************************************************************************************************************
void anima(int animacao, int plot,int lx, int ly,int lz,double ****m)
{
  int i,j,k;
  long int t;
  if(animacao == 1){
	 if(t%ns==0){
		printf("set xrange[-1:%d]\n",Lx);
		printf("set yrange[-1:%d]\n",Ly);
		printf("set zrange[-1:%d]\n",Lz);
		printf("set cbrange [-1:1]\n");
		printf("unset key\n");
		printf("set palette defined ( 0 0 1 0, 0.3333 0 0 1, 0.6667 1 0 0,1 1 0.6471 0 )\n");


		if(plot == 1)
		  printf("sp '-' u 1:2:3:4:5:6 with vectors head size 0.08,20,60 filled lc palette \n");
		if(plot == 2)
		  printf("sp '-' u 1:2:3:4:5:6 with vectors head size 0.08,20,10 filled \n");
		if(plot == 0)
		  printf("p '-' u ($3==0?$1:1/0):2:4:5:6  with vectors head size 0.5,15,60 filled lc palette\n");

		for(i=0;i<lx;i++){
		  for(j=0;j<ly;j++){
			 for(k=0;k<lz;k++){
				printf("%d %d %d %f %f %f\n",i,j,k,m[0][i][j][k],m[1][i][j][k],m[2][i][j][k]);
			 } } }
			 printf("e\n");
	 }
	 return;
  }
}
//**************************************************************************************************************
//**************************************************************************************************************
//**************************************************************************************************************
void snapshot(int snap, int lx, int ly, int lz, double ****m, long int t)
{
  int i,j,k;
  FILE *f5;
  char nome[1000];
  if(snap == 1){
	 if(t == 0 ||t == 6000 || t == tmax-1){
		sprintf(nome,"config-tempo%ld.dat",t);
		f5 = fopen(nome,"w");
		for(i=0; i<lx; i++){
		  for(j=0; j<ly ; j++){
			 for(k=0; k<lz ; k++){
				fprintf(f5,"%d %d %d %f %f %f\n",i,j,k,m[0][i][j][k], m[1][i][j][k], m[2][i][j][k]);
			 } } }
	 }
	 return;
	 fclose(f5);
  }

}
//**************************************************************************************************************
//**************************************************************************************************************
//**************************************************************************************************************
void configuracao(int orientacao, int lx, int ly, int lz, double ****m)
{
  int i,j,k, DD;
  if(orientacao == 1){
	 for(i=0; i<lx; i++){
		for(j=0; j<ly ; j++){
		  for(k=0; k<lz ; k++){
			 double ang2=FRANDOM*2*PI;	    	//para que a magnetização inicie com
			 double ang1=(2*FRANDOM-1)*PI;	//orientação aleatória
			 if(FRANDOM < dil)
				DD=0;
				else
				  DD=1;
			 m[0][i][j][k] = DD * sin(ang1) * cos(ang2);
			 m[1][i][j][k] = DD * sin(ang1) * sin(ang2);
			 m[2][i][j][k] = DD * cos(ang1);
	 }	} }
  }
  else{
	 for(i=0; i<lx; i++){
		for(j=0; j<ly ; j++){
		  for(k=0; k<lz ; k++){
			 if(FRANDOM < dil)
				DD=0;
			 else
				DD=1;


			 m[0][i][j][k] = -3.162269754e-3 * DD;
			 m[1][i][j][k] = 3.162269754e-3 * DD;
			 m[2][i][j][k] = 0.99999 * pow(1,k+j+i) * DD;




	 }	} }
  }
  return;
}
//**************************************************************************************************************
//**************************************************************************************************************
//**************************************************************************************************************
void update(double ****m, double ****h,double ****d)
{
  double ****ff,****fft,****gg,****ggt, ****magt, ****magnew;
  int hc,i,j,k;

  ff      				= (double****)calloc(co,sizeof(double***)*co);
  fft    				= (double****)calloc(co,sizeof(double***)*co);
  gg     				= (double****)calloc(co,sizeof(double***)*co);
  ggt     				= (double****)calloc(co,sizeof(double***)*co);
  magt      			= (double****)calloc(co,sizeof(double***)*co);
  magnew     			= (double****)calloc(co,sizeof(double***)*co);
  for(hc = 0; hc < co; hc++ ){
	 ff[hc]      			= (double***)calloc(Lx,sizeof(double**)*Lx);
	 fft[hc]      		= (double***)calloc(Lx,sizeof(double**)*Lx);
	 gg[hc]      			= (double***)calloc(Lx,sizeof(double**)*Lx);
	 ggt[hc]      		= (double***)calloc(Lx,sizeof(double**)*Lx);
	 magt[hc]      		= (double***)calloc(Lx,sizeof(double**)*Lx);
	 magnew[hc]      	= (double***)calloc(Lx,sizeof(double**)*Lx);
	 for(i = 0; i < Lx; i++ ){
		ff[hc][i]      		= (double**)calloc(Ly,sizeof(double*)*Ly);
		fft[hc][i]      	= (double**)calloc(Ly,sizeof(double*)*Ly);
		gg[hc][i]      		= (double**)calloc(Ly,sizeof(double*)*Ly);
		ggt[hc][i]      	= (double**)calloc(Ly,sizeof(double*)*Ly);
		magt[hc][i]      	= (double**)calloc(Ly,sizeof(double*)*Ly);
		magnew[hc][i]      = (double**)calloc(Ly,sizeof(double*)*Ly);
		for(j = 0; j < Ly; j++ ){
		  ff[hc][i][j]      	= (double*)calloc(Lz,sizeof(double)*Lz);
		  fft[hc][i][j]      = (double*)calloc(Lz,sizeof(double)*Lz);
		  gg[hc][i][j]      	= (double*)calloc(Lz,sizeof(double)*Lz);
		  ggt[hc][i][j]      = (double*)calloc(Lz,sizeof(double)*Lz);
		  magt[hc][i][j]     = (double*)calloc(Lz,sizeof(double)*Lz);
		  magnew[hc][i][j]	= (double*)calloc(Lz,sizeof(double)*Lz);
		}}}

  f(m,h,ff);
  // printf("*****UPDATE - f*****\n");
  g(m,d,gg);
  // printf("*****UPDATE - g*****\n");

  for(hc=0;hc<co;hc++)
	 for(i=0;i<Lx;i++)
		for(j=0;j<Ly;j++)
		  for(k=0;k<Lz;k++)
			 magt[hc][i][j][k] = m[hc][i][j][k] + ff[hc][i][j][k]*dt + gg[hc][i][j][k];

		  f(magt,h,fft);
		// printf("*****UPDATE - ff*****\n");
		g(magt,d,ggt);
	 // printf("*****UPDATE - gg*****\n");
	 for(hc=0;hc<co;hc++)
		for(i=0;i<Lx;i++)
		  for(j=0;j<Ly;j++)
			 for(k=0;k<Lz;k++)
				magnew[hc][i][j][k] = m[hc][i][j][k] + 0.5*(ff[hc][i][j][k]+fft[hc][i][j][k])*dt +
				0.5*(ggt[hc][i][j][k]+gg[hc][i][j][k]);
			 for(hc=0;hc<co;hc++)
				for(i=0;i<Lx;i++)
				  for(j=0;j<Ly;j++)
					 for(k=0;k<Lz;k++)
						m[hc][i][j][k] = magnew[hc][i][j][k];

  for(i = 0; i < co; i++ ){
	 for(j = 0; j < Lx; j++ ){
		for(k = 0; k < Ly; k++ ){

		  free(ff[i][j][k]);
		  free(fft[i][j][k]);
		  free(gg[i][j][k]);
		  free(ggt[i][j][k]);
		  free(magt[i][j][k]);
		  free(magnew[i][j][k]);
		}

		free(ff[i][j]);
		free(fft[i][j]);
		free(gg[i][j]);
		free(ggt[i][j]);
		free(magt[i][j]);
		free(magnew[i][j]);
	 }

	 free(ff[i]);
	 free(fft[i]);
	 free(gg[i]);
	 free(ggt[i]);
	 free(magt[i]);
	 free(magnew[i]);
  }
  free(ff);
  free(fft);
  free(gg);
  free(ggt);
  free(magt);
  free(magnew);

  return;
}
