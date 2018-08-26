#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include "libbmp/libbmp.h"

#define _USE_MATH_DEFINES

int L=1000;
int MODE=0;
int WINDOWS_HEIGHT=400;
int WINDOWS_WIDTH=500;

int para;
double zoomStep=0.2;
double zoom1=1,zoom2=1;
double horizontal=0, vertical=0;
int mouseLeftClick=0;
double mouseX, mouseY;
double mouseXant, mouseYant;
double corner_x1=0, corner_y1=0;
double corner_x2=0, corner_y2=0;
int flag;

typedef struct{
  GLfloat x, y, z;
  GLfloat r, g, b, a;
} Vertex;

typedef struct{
  GLfloat x, y, z;
} Data;

void drawPoint(Vertex v1, GLfloat size);
void drawPointsDemo(int width, int height);
void drawLineSegment(Vertex v1, Vertex v2, GLfloat width);
void DrawCircle(double x0, double y0, double r);
void drawGrid(GLfloat width, GLfloat height, GLfloat grid_width);
void DrawFrame();

void gridMode(int tempo, double **GRID);
void particleMode(int tempo, double **GRID);
void grid2dMode(int tempo, double **GRID);
void particle2dMode(int tempo, double **GRID);
void SpectreMode();
void ArrowMode(int tempo);

void cursorPositionCallback( GLFWwindow *window, double xPos, double yPos );
void mouseButtonCallback( GLFWwindow *window, int button, int action, int mods );
void keyCallback( GLFWwindow *window, int key, int scancode, int action, int mods );
void scrollCallback( GLFWwindow *window, double xOffset, double yOffset );
void mouseTranslate();
void screenshot();

int NCOLORS=2;
double *R,*G,*B;

void readColorFile(char *optarg){
  int ch,i;
  FILE *f1=fopen(optarg,"r");

  NCOLORS=0;
  while(!feof(f1)){ // Conta o número de linhas
    ch = fgetc(f1);
    if(ch == '\n'){
      NCOLORS++;
    }
  }

  R=malloc(NCOLORS*sizeof(double));
  G=malloc(NCOLORS*sizeof(double));
  B=malloc(NCOLORS*sizeof(double));

  char hex[6];
  int r,g,b;
  rewind(f1);
  for(i=0; i<NCOLORS; i++){
    fscanf(f1,"%*2s%s",hex);
    sscanf(hex, "%02x%02x%02x", &r, &g, &b);
    R[i]=r/255.; G[i]=g/255.; B[i]=b/255.;
  }
  fclose(f1);
}

int main(int argc, char *argv[]){
  GLFWwindow* window;

  R=malloc(NCOLORS*sizeof(double));
  G=malloc(NCOLORS*sizeof(double));
  B=malloc(NCOLORS*sizeof(double));
  R[0]=1.0; R[1]=0.0;
  G[0]=1.0; G[1]=0.0;
  B[0]=1.0; B[1]=0.0;
  
  while(1){
    static struct option long_options[] = {
      {"mode", required_argument, 0, 'm'},
      {"height", required_argument, 0, 'h'},
      {"width", required_argument, 0, 'w'},
      {"color", required_argument, 0, 'c'},
      {0, 0, 0, 0}
    };

    // getopt_long stores the option index here.
    int option_index = 0;

    int c = getopt_long(argc, argv, "l:m:h:w:c:", long_options, &option_index);

    // Detect the end of the options.
    if (c == -1) { break; }

    switch (c) {
    case 0:
      break;

    case 'l':
      L=atoi(optarg);
      break;

    case 'm':
      MODE=atoi(optarg);
      break;

    case 'h':
      WINDOWS_HEIGHT=atoi(optarg);
      break;

    case 'w':
      WINDOWS_WIDTH=atoi(optarg);
      break;

    case 'c':
      readColorFile(optarg);
      break;

    default:
      return 1;
    }
  }
  
  if (!glfwInit()){
    exit(EXIT_FAILURE);
  }
  window = glfwCreateWindow(WINDOWS_WIDTH, WINDOWS_HEIGHT,"Dynamic Simulator", NULL, NULL);
  if (!window){
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glEnable(GL_POINT_SMOOTH);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  int i,j;
  double **GRID;

  if(MODE==0 || MODE==1){
    GRID=(double **)malloc(600*sizeof(double *));
    for (i = 0; i < 600; ++i){
      GRID[i]=(double *)malloc(L*sizeof(double));
    }
  }
  else if(MODE==2){
    GRID=(double **)malloc(L*sizeof(double *));
    for (i = 0; i < L; ++i){
      GRID[i]=(double *)malloc(L*sizeof(double));
    }
  }
  else if(MODE==3){
    GRID=(double **)malloc(L*sizeof(double *));
    for (i = 0; i < L; ++i){
      GRID[i]=(double *)malloc(3*sizeof(double));
    }
  }

  glfwSetCursorPosCallback( window, cursorPositionCallback );
  glfwSetMouseButtonCallback( window, mouseButtonCallback );
  glfwSetKeyCallback( window, keyCallback );
  glfwSetScrollCallback( window, scrollCallback );
  
  int tempo = 0;
  while (!glfwWindowShouldClose(window)){
    float ratio;
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = (float) width / (float)height;
    glViewport(0, 0, width, height);

    glLoadIdentity();
    if(MODE==0 || MODE==1){ glScalef(zoom1,1,0); }
    if(MODE==2 || MODE==3){ glScalef(zoom1,zoom2,0); }
    mouseTranslate();
    glTranslatef(horizontal, vertical, 0);

    DrawFrame();
    if(para==0){ tempo++; }
    
    if(MODE==0){
      gridMode(tempo,GRID);
    }
    else if(MODE==1){
      particleMode(tempo,GRID);
    }
    else if(MODE==2){
      grid2dMode(tempo,GRID);
    }
    else if(MODE==3){
      particle2dMode(tempo, GRID);
    }
    else if(MODE==4){
      SpectreMode(tempo);
    }
    else if(MODE==5){
      ArrowMode(tempo);
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  free(GRID);
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void mouseTranslate(){
  if(mouseLeftClick==1){
    if(flag==1){
      horizontal+=2*(mouseX-mouseXant)/WINDOWS_WIDTH/zoom1;
      vertical+=2*(mouseYant-mouseY)/WINDOWS_HEIGHT/zoom2;
    }
    flag=1;
    mouseXant=mouseX;
    mouseYant=mouseY;
  }
  else{
    flag=0;
  }
}

void drawPoint(Vertex v1, GLfloat size){
  glPointSize(size);
  glBegin(GL_POINTS);
  glColor4f(v1.r, v1.g, v1.b, v1.a);
  glVertex3f(v1.x, v1.y, v1.z);
  glEnd();
}

void DrawCircle(double x0, double y0, double r){
  int i;

  glBegin(GL_POLYGON);
  for(i=0; i <= 300; i++){
    double angle = 2 * M_PI * i / 300;
    double x = r/880*cos(angle)-x0;
    double y = r/660*sin(angle)-y0;
    glVertex2d(x,y);
  }
  glEnd();
}

void drawPointsDemo(int width, int height){
  GLfloat size=5.0f;
  for(GLfloat x = 0.0f; x<=1.0f; x+=0.2f, size+=5)
    {
      Vertex v1 = {x, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
      drawPoint(v1, size);
    }
}

void drawLineSegment(Vertex v1, Vertex v2, GLfloat width) {
  glLineWidth(width);
  glBegin(GL_LINES);
  glVertex3f(v1.x, v1.y, v1.z);
  glColor4f(v2.r, v2.g, v2.b, v2.a);
  glVertex3f(v2.x, v2.y, v2.z);
  glEnd();
}

void drawGrid(GLfloat width, GLfloat height, GLfloat grid_width){
  //horizontal lines
  for(float i=-height; i<height; i+=grid_width){
    Vertex v1 = {-width, i, 0.0f, 1.0f, 1.0f, 1.0f, 0.5f};
    Vertex v2 = {width, i, 0.0f, 1.0f, 1.0f, 1.0f, 0.5f};
    drawLineSegment(v1, v2,grid_width);
  }
  //vertical lines
  for(float i=-width; i<width; i+=grid_width){
    Vertex v1 = {i, -height, 0.0f, 1.0f, 1.0f, 1.0f, 0.5f};
    Vertex v2 = {i, height, 0.0f, 1.0f, 1.0f, 1.0f, 0.5f};
    drawLineSegment(v1, v2,grid_width);
  }
}

void DrawFrame(){
  Vertex v1 = {-0.95f, -0.95f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
  Vertex v2 = {0.95f, -0.95f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
  Vertex v3 = {0.95f, 0.95f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
  Vertex v4 = {-0.95f, 0.95f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
}

void gridMode(int tempo,double **GRID){
  int i,j,k;
  float cor;
  float yy;
  Vertex v;

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
	  v.r = R[k]+(R[k+1]-R[k])*GRID[j][i];
	  v.g = G[k]+(G[k+1]-G[k])*GRID[j][i];
	  v.b = B[k]+(B[k+1]-B[k])*GRID[j][i];
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
	  v.r = R[k]+(R[k+1]-R[k])*GRID[i][j];
	  v.g = G[k]+(G[k+1]-G[k])*GRID[i][j];
	  v.b = B[k]+(B[k+1]-B[k])*GRID[i][j];
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
  length = (600.0/(2*L))/660;
  cor = 1.0f;
  glClear(GL_COLOR_BUFFER_BIT);
  for (i = 0;i< L;i++){
    for(j= 0;j<L;j++){
      scanf("%lf %lf %lf\n", &xx, &yy, &zz);
      cor = (zz+1.0)/2.0;
      v1.x = (i - L/2)/(1.1*L/2);
      v1.y = (j - L/2)/(1.1*L/2);
      v1.z = 0.0f;
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
      drawLineSegment(v1,v2,3.0f);
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
    drawLineSegment(v1,v2,3.0f);
    v1.x = v2.x;
    v1.y = v2.y;
    v1.z = v2.z;
    v1.r = cor;
    v1.g = cor;
    v1.b = cor;
    v1.a = 1.0f;
  }
}

void cursorPositionCallback( GLFWwindow *window, double xPos, double yPos ){
  mouseX=xPos;
  mouseY=yPos;
}

void mouseButtonCallback( GLFWwindow *window, int button, int action, int mods ){
  if( button==0 && action==GLFW_PRESS ){ mouseLeftClick=1; }
  else if( button==0 && action==GLFW_RELEASE ){ mouseLeftClick=0; }

  if( button==1 && action==GLFW_PRESS ){
    corner_x1=mouseX;
    corner_y1=mouseY;
  }
  else if( button==1 && action==GLFW_RELEASE ){
    corner_x2=mouseX;
    corner_y2=mouseY;
    horizontal=-(corner_x2+corner_x1-WINDOWS_WIDTH)/WINDOWS_WIDTH;
    vertical=(corner_y2+corner_y1-WINDOWS_HEIGHT)/WINDOWS_HEIGHT;

    zoom1+=(1+WINDOWS_WIDTH/fabs(corner_x2-corner_x1))*zoom1;
    zoom2+=(1+WINDOWS_HEIGHT/fabs(corner_y2-corner_y1))*zoom2;
  }
}

void keyCallback( GLFWwindow *window, int key, int scancode, int action, int mods ){
  if (key == 32 && action == GLFW_PRESS){ // space
    if(para==0){
      para=1;
    }
    else if(para==1){
      para=0;
    }
  }

  if (key == 262 && (action == GLFW_REPEAT || GLFW_PRESS)){ horizontal-=0.08; } // Right arrow
  if (key == 263 && (action == GLFW_REPEAT || GLFW_PRESS)){ horizontal+=0.08; } // Left arrow
  if (key == 264 && (action == GLFW_REPEAT || GLFW_PRESS)){ vertical+=0.08; } // Down arrow
  if (key == 265 && (action == GLFW_REPEAT || GLFW_PRESS)){ vertical-=0.08; } // Up arrow

  if (key == 65 && action == GLFW_PRESS){ // Letra a - autoscale
    horizontal=0;
    vertical=0;
    zoom1=1;
    zoom2=1;
  }

  if (key == 45 && (action == GLFW_REPEAT || GLFW_PRESS)){ // Tecla -
    zoom1-=zoomStep*zoom1;
    zoom2-=zoomStep*zoom2;
  }
  if (key == 61 && (action == GLFW_REPEAT || GLFW_PRESS)){ // Tecla +
    zoom1+=zoomStep*zoom1;
    zoom2+=zoomStep*zoom2;
  }

  if (key == 83 && action == GLFW_PRESS){ // Letra s - screenshot
    screenshot();
  }
}

void scrollCallback( GLFWwindow *window, double xOffset, double yOffset ){
  zoom1+=yOffset*zoomStep*zoom1;
  zoom2+=yOffset*zoomStep*zoom2;
  glClear(GL_COLOR_BUFFER_BIT);
}

void screenshot(){
  bmp_img img;
  bmp_img_init_df(&img, WINDOWS_WIDTH, WINDOWS_HEIGHT);

  for (size_t y = 0; y < WINDOWS_HEIGHT; y++){
    for (size_t x = 0; x < WINDOWS_WIDTH; x++){
      GLfloat array[3];
      glReadPixels(x, WINDOWS_HEIGHT-y, 1, 1, GL_RGB, GL_FLOAT, array);
      bmp_pixel_init(&img.img_pixels[y][x], 254*array[0], 254*array[1], 254*array[2]);
    }
  }

  bmp_img_write(&img, "screenshot.bmp");
  bmp_img_free(&img);
}
