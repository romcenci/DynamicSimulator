#include "modes.h"

void gridMode(int tempo,double **GRID, HSV_Color *hsv_pal){
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

  for(j=0;j < 600; j++){
    for(i=0; i<L; i++){
      v.x = (i - L/2)/(1.1*L/2);
      v.y = (float) (-j+300.0f)/330.0f;
      v.z = 0.0f;

      for(k=0; k<NCOLORS-1; k++){
	if(GRID[j][i]>=(double)k/(NCOLORS-1) && GRID[j][i]<=(double)(k+1.)/(NCOLORS-1)){
	  v.r = pal[k].r+(pal[k+1].r-pal[k].r)*GRID[j][i];
	  v.g = pal[k].g+(pal[k+1].g-pal[k].g)*GRID[j][i];
	  v.b = pal[k].b+(pal[k+1].b-pal[k].b)*GRID[j][i];
	}
      }
      v.a = 1.0f;
      drawPoint(v,1.0f);
    }
  }
}

void particleMode(int tempo,double **GRID){
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

void grid2dMode(int tempo,double **GRID){
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
      v.x = (i*(2./L)-1)*660/880;
      v.y = (j*(2./L)-1);
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

void particle2dMode(int tempo,double **GRID){
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

void ArrowMode(int tempo){
  int i,j;
  double xx,yy,zz;
  double length, cor;

  Vertex v1,v2;
  length = (600.0/(2.0*L))/660.0;
  cor = 1.0f;
  glClear(GL_COLOR_BUFFER_BIT);

  for (i = 0;i<L;i++){
    for(j= 0;j<L;j++){
      scanf("%lf %lf %lf\n", &xx, &yy, &zz);
      cor = (zz+1.0)/2.0;
      v1.x = (i - L/2)/(1.1*L/2);
      v1.y = (j - L/2)/(1.1*L/2);
      v1.z = 0.0;
      v1.r = cor;
      v1.g = cor;
      v1.b = 1.0f;
      v1.a = 1.0f;
      v2.x = (i - L/2)/(1.1*L/2)+length*xx;
      v2.y = (j - L/2)/(1.1*L/2)+length*yy;
      v2.z = length*zz;
      v2.r = cor;
      v2.g = cor;
      v2.b = 1.0f;
      v2.a = 1.0f;
    //   Vertex tp1 = {1*10*length,0.0f,1.0f,v1.r,v1.g,v1.b,v1.a};
    //   Vertex tp2 = {0.8*10*length,-0.5*10*length,1.0f,v1.r,v1.g,v1.b,v1.a};
    //   Vertex tp3 = {0.8*10*length,+0.5*10*length,1.0f,v1.r,v1.g,v1.b,v1.a};
      // glBegin(GL_TRIANGLES);
    // glColor3f(0.5, 0.5, 0.5);
    // glVertex3f(tp1.x, tp1.y, tp1.z);
    // glVertex3f(tp2.x, tp2.y, tp2.z);
    // glVertex3f(tp3.x, tp3.y, tp3.z);
    // glEnd();
      DrawArrow2D(v1,v2,1.0f);
    }
  }
  /* v1.x=0; */
  /* v1.y=0; */
  /* v2.x=0.2; */
  /* v2.y=0.5;   */
  /* DrawArrow(v1,v2,3.0f); */
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
