#define FRANDOM (rand()/(RAND_MAX+1.0))

double ngaussian(void){  //Numerical Recipes 
  static int iset=0;
  static double gset;
  double fac,r,v1,v2;
  
  if (iset==0) {
    do {
      v1=2.0*FRANDOM-1.0;
      v2=2.0*FRANDOM-1.0;
      r=v1*v1+v2*v2;
    } 
    while (r>=1 || r==0.0);
    fac=sqrt(-2.0*log(r)/r);
    gset=v1*fac;
    iset=1;
    return v2*fac;
  }
  else {
    iset=0;
    return gset;
  }
}

double min(double x, double y){
  return (fabs(x)<fabs(y) ? x : y);
}

void quick_sort(double *x, int esq, int dir){
  int i, j;
  double meio, tmp;
  i=esq;
  j=dir;
  meio=x[(esq+dir)/2];
     
  while(i<=j){
    while(x[i]<meio && i<dir){i++;}
    while(x[j]>meio && j>esq){j--;}
    if(i<=j){
      tmp=x[i];
      x[i]=x[j];
      x[j]=tmp;
    }
    i++;
    j--;
  }
     
  if(j>esq){quick_sort(x, esq, j);}
  if(i<dir){quick_sort(x, i, dir);}
}
