#include "modes.h"

void gridMode(int tempo, double **GRID, HSV_Color *hsv_pal){
  int i,j,k;
  float cor;
  float yy;
  Vertex v;
  HSV_Color hsv;
  RGB_Color rgb;
  double r,g,b;

  for(i=0; i<L; i++){
    if(para==0){
      scanf("%f\n", &cor);
      cor = (cor+1.)/2.0;
      GRID[tempo%600][i] = cor;
    }
  }
  
  glClear(GL_COLOR_BUFFER_BIT);
  for(j=0;j < 600; j++){
    for(i=0; i<L; i++){
      v.x = (i - L/2)/(1.1*L/2);
      v.y = (float) (-j+300.0f)/330.0f;
      v.z = 0.0f;

      for(k=0; k<NCOLORS-1; k++){
	if(GRID[j][i]>=(double)k/(NCOLORS-1) && GRID[j][i]<=(double)(k+1.)/(NCOLORS-1)){
	  rgb.r=v.r;
	  rgb.g=v.g;
	  rgb.b=v.b;

	  RGB2HSV(&rgb, &hsv, 1);

	  hsv.h = hsv_pal[k].h+(hsv_pal[k+1].h-hsv_pal[k].h)*GRID[j][i];
	  hsv.s = hsv_pal[k].s+(hsv_pal[k+1].s-hsv_pal[k].s)*GRID[j][i];
	  hsv.v = hsv_pal[k].v+(hsv_pal[k+1].v-hsv_pal[k].v)*GRID[j][i];

	  HSV2RGB(&rgb, &hsv, 1);

	  v.r=rgb.r;
	  v.g=rgb.g;
	  v.b=rgb.b;
	}
      }
      v.a = 1.0f;
      drawPoint(v,1.0f);
    }
  }
}

void particleMode(int tempo, double **GRID){
  int i,j;
  float cor;
  float yy, xx;
  Vertex v;

  for(i=0; i<L; i++){
    if(para==0){
      scanf("%f\n", &xx);
    }
    GRID[tempo%600][i]=xx;
  }

  yy = (float) (-(tempo%600)+300.0f)/330.0f;
  glClear(GL_COLOR_BUFFER_BIT);
  for(j=0; j<600; j++){
    for(i=0; i<L; i++){
      if(GRID[j][i]!=0){
	cor = 1.0f;
	v.x = (2*GRID[j][i]-1)/1.15;
	v.y = (float) (-j+300.0f)/330.0f;
	v.z = 0.0f;
	v.r = cor;
	v.g = cor;
	v.b = cor;
	v.a = 1.0f;
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	drawPoint(v,1.0f);
      }
    }
  }
}

void grid2dMode(int tempo, double **GRID){
  int i,j,k;
  double cor;
  Vertex v;

  glClear(GL_COLOR_BUFFER_BIT);

  for(j=0; j<L; j++){
    for(i=0; i<L; i++){
      if(para==0){
  	scanf("%lf\n", &cor);
  	cor = (cor+1.0)/2.0;
  	GRID[i][j]=cor;
      }
      v.x = (i*(2./(L-1))-1)*WINDOWS_HEIGHT/WINDOWS_WIDTH;
      v.y = (j*(2./(L-1))-1);
      v.z = 0.0f;
      for(k=0; k<NCOLORS-1; k++){
	if(GRID[i][j]>=(double)k/(NCOLORS-1) && GRID[i][j]<=(double)(k+1.)/(NCOLORS-1)){
	  v.r = pal[k].r+(pal[k+1].r-pal[k].r)*GRID[i][j];
	  v.g = pal[k].g+(pal[k+1].g-pal[k].g)*GRID[i][j];
	  v.b = pal[k].b+(pal[k+1].b-pal[k].b)*GRID[i][j];
	}
      }
      v.a = 1.0f;
      drawPoint(v,1000/L);
    }
  }
}

void particle2dMode(int tempo, double **GRID){
  int i;
  double cor, xx, yy, r;
  Vertex v;

  glClear(GL_COLOR_BUFFER_BIT);

  for(i=0; i<L; i++){
    if(para==0){
      scanf("%lf %lf %lf\n", &xx, &yy, &r);
      GRID[i][0]=xx;
      GRID[i][1]=yy;
      GRID[i][2]=r;
    }
    cor = 1.0;
    v.x = (2*GRID[i][0]-1)/1.15*660/880;
    v.y = (2*GRID[i][1]-1)/1.15;
    v.z = 0.0f;
    v.r = cor;
    v.g = cor;
    v.b = cor;
    v.a = 1.0f;
    DrawCircle(v.x, v.y, GRID[i][2]+3);
  }
}

void ArrowMode(int tempo, double **GRID){
  int i,j;
  double xx,yy,zz;
  double length, cor;

  Vertex v1,v2;
  length = (600.0/(2.0*L))/660.0;
  cor = 1.0f;
  glClear(GL_COLOR_BUFFER_BIT);

  for (i = 0;i<L;i++){
    for(j= 0;j<L;j++){
      if(para==0){
	scanf("%lf %lf %lf\n", &xx, &yy, &zz);
	GRID[i+L*j][0]=xx;
	GRID[i+L*j][1]=yy;
	GRID[i+L*j][2]=zz;
      }
      cor = (GRID[i+L*j][2]+1.0)/2.0;
      v1.x = 0.85*(i*(2./(L-1))-1)*WINDOWS_HEIGHT/WINDOWS_WIDTH;
      v1.y = 0.85*(j*(2./(L-1))-1);
      v1.z = 0.0;
      v1.r = cor;
      v1.g = cor;
      v1.b = 1.0f;
      v1.a = 1.0f;
      v2.x = 0.85*(i*(2./(L-1))-1)*WINDOWS_HEIGHT/WINDOWS_WIDTH+1.3*length*GRID[i+L*j][0];
      v2.y = 0.85*(j*(2./(L-1))-1)+1.3*length*GRID[i+L*j][1];
      v2.z = 1.3*length*GRID[i+L*j][2];
      v2.r = cor;
      v2.g = cor;
      v2.b = 1.0f;
      v2.a = 1.0f;
      DrawArrow(v1,v2,2.0f);
    }
  }
}

void SpectreMode(){
  int pos,t;
  float value;
  double cor = 1.0;
  Vertex v1,v2;
  int Lk = (800/2+1);
  scanf("%d\t%f",&pos,&value);
  v1.x = (2.0*pos/Lk) -1.0f;
  v1.y = (value);
  v1.z = 0.0f;
  v1.r = cor;
  v1.g = cor;
  v1.b = cor;
  v1.a = 1.0f;
  glClear(GL_COLOR_BUFFER_BIT);

  for(t = 1; t < Lk;t++){
    scanf("%d",&pos);
    scanf("%f",&value);
    v2.x = (2.0f*t/Lk)-1.0f;
    // v2.y = 2.0f*rand()/RAND_MAX - 1.0f;
    v2.y = value +.7;
    v2.z = 0.0f;
    v2.r = cor;
    v2.g = cor;
    v2.b = cor;
    v2.a = 1.0f;
    drawLineSegment(v1,v2,2.0f);
    v1.x = v2.x;
    v1.y = v2.y;
    v1.z = v2.z;
    v1.r = cor;
    v1.g = cor;
    v1.b = cor;
    v1.a = 1.0f;
  }
}
