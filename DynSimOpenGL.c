#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "libbmp/libbmp.h"

#define _USE_MATH_DEFINES 

int L=TAM;
int para;

double zoomStep=0.2;
double zoom=1;
double horizontal=0, vertical=0;
int mouseClick=0;
double mouseX, mouseY;
double mouseXant, mouseYant;
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
void drawGrid(GLfloat width, GLfloat height, GLfloat grid_width);
void DrawFrame();
void gridMode(int tempo, double GRID[600][800]);
void particleMode(int tempo);
void grid2dMode();
void particle2dMode();
void DrawCircle(double x0, double y0, double r);
void SpectreMode();
void cursorPositionCallback( GLFWwindow *window, double xPos, double yPos );
void mouseButtonCallback( GLFWwindow *window, int button, int action, int mods );
void keyCallback( GLFWwindow *window, int key, int scancode, int action, int mods );
void scrollCallback( GLFWwindow *window, double xOffset, double yOffset );
void mouseTranslate();
void screenshot();

int main(void){
  GLFWwindow* window;
  
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
  double GRID[600][L];
  int i,j;
  for (i = 0; i < 600; i++){
    for(j = 0; j< L; j++){
      GRID[i][j] = 0.0;
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
    mouseTranslate();
    glTranslatef(0.08*horizontal, 0.08*vertical, 0);
    // drawGrid(5.0f, 1.0f, 0.1f);
    
    if(para==0){
      DrawFrame();
      tempo++;

      if(mo==0){
	glScalef(zoom,1,0);
	gridMode(tempo,GRID);
      }
      else if(mo==1){
	glScalef(zoom,1,0);
	particleMode(tempo);
      }
      else if(mo==2){
        glScalef(zoom,zoom,0);
	glClear(GL_COLOR_BUFFER_BIT);
	grid2dMode(tempo);
      }
      else if(mo==3){
	glScalef(zoom,zoom,0);
	mouseTranslate();
	glTranslatef(0.05*horizontal, 0.05*vertical, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	particle2dMode(tempo);
      }
      else if(mo==4){
	SpectreMode(tempo);
      }

      glfwSwapBuffers(window);
    }
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void mouseTranslate(){
  if(mouseClick==1){
    if(flag==1){
      horizontal+=0.04*(mouseX-mouseXant);
      vertical+=0.04*(mouseYant-mouseY);
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

void gridMode(int tempo,double GRID[600][L]){
  int i,j;
  float cor;
  float yy;
  Vertex v;

  for(i=0; i<L; i++){
    scanf("%f\n", &cor);
    cor = (cor+1)/2.0;
    GRID[tempo%600][i] = cor;
  }
  glClear(GL_COLOR_BUFFER_BIT);
  for(j=0;j < 600; j++){
    for(i=0; i<L; i++){
      v.x = (i - L/2)/(1.1*L/2);
      v.y = (float) (-j+300.0f)/330.0f;
      v.z = 0.0f;
      v.r = GRID[j][i];
      v.g = GRID[j][i];
      v.b = GRID[j][i];
      v.a = 1.0f;
      drawPoint(v,1.0f);
    }
  }
}

void particleMode(int tempo){
  int i;
  float cor;
  float yy, xx;
  Vertex v;
  
  glBegin(GL_QUADS);
  glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
  glVertex3f(-1.0f, -(2.*(tempo%600)-600.)/660.-0.03f, 0.0f);
  glVertex3f(1.0f,  -(2.*(tempo%600)-600.)/660.-0.03f, 0.0f);
  glVertex3f(1.0f,  -(2.*(tempo%600)-600.)/660., 0.0f);
  glVertex3f(-1.0f, -(2.*(tempo%600)-600.)/660., 0.0f);
  glEnd();

  yy = (float) (-(tempo%600)+300.0f)/330.0f;
  for(i=0; i<L; i++){
    scanf("%f\n", &xx);
    cor = 1.0f;
    v.x = (2*xx-1)/1.15;
    v.y = yy;
    v.z = 0.0f;
    v.r = cor;
    v.g = cor;
    v.b = cor;
    v.a = 1.0f;

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    drawPoint(v,1.1f);
  }
}

void grid2dMode(int tempo){
  int i,j;
  double cor;
  Vertex v;

  for(j=0; j<L; j++){
    for(i=0; i<L; i++){
      scanf("%lf\n", &cor);
      cor = (cor+1.0)/2.0;
      v.x = (i*(2./L)-1)*660/880;
      v.y = (j*(2./L)-1);
      v.z = 0.0f;
      v.r = cor;
      v.g = cor;
      v.b = cor;
      v.a = 1.0f;
      drawPoint(v,1000/L);
    }
  }
}

void particle2dMode(){
  int i;
  double cor, xx, yy, r;
  Vertex v;

  glClear(GL_COLOR_BUFFER_BIT);

  for(i=0; i<L; i++){
    scanf("%lf %lf %lf\n", &xx, &yy, &r);
    cor = 1.0;
    v.x = (2*xx-1)/1.15*660/880;
    v.y = (2*yy-1)/1.15;
    v.z = 0.0f;
    v.r = cor;
    v.g = cor;
    v.b = cor;
    v.a = 1.0f;
    DrawCircle(v.x, v.y, r+3);
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
  if( button==0 && action==GLFW_PRESS ){
    mouseClick=1;
  }
  else if( button==0 && action==GLFW_RELEASE ){
    mouseClick=0;    
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

  if (key == 262 && (action == GLFW_REPEAT || GLFW_PRESS)){ horizontal--; } // Right arrow
  if (key == 263 && (action == GLFW_REPEAT || GLFW_PRESS)){ horizontal++; } // Left arrow
  if (key == 264 && (action == GLFW_REPEAT || GLFW_PRESS)){ vertical++; } // Down arrow
  if (key == 265 && (action == GLFW_REPEAT || GLFW_PRESS)){ vertical--; } // Up arrow

  if (key == 65 && action == GLFW_PRESS){ // Letra a - autoscale
    horizontal=0;
    vertical=0;
    zoom=1;
  }

  if (key == 45 && (action == GLFW_REPEAT || GLFW_PRESS)){ zoom-=zoomStep*zoom; } // Tecla -
  if (key == 61 && (action == GLFW_REPEAT || GLFW_PRESS)){ zoom+=zoomStep*zoom; } // Tecla +

  if (key == 83 && action == GLFW_PRESS){ // Letra s - screenshot
    screenshot();
  }
}

void scrollCallback( GLFWwindow *window, double xOffset, double yOffset ){
  zoom+=yOffset*zoomStep*zoom;
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
