#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include <string.h>

#define	GLFW_ARROW_CURSOR 0x00036001
#define GLFW_HAND_CURSOR 0x00036004
#define GLFW_VRESIZE_CURSOR 0x00036006
#define GLFW_CROSSHAIR_CURSOR 0x00036003

int flag;
int mouseLeftClick=0;
double zoomStep=0.2;
double mouseX, mouseY;
double mouseXant, mouseYant;
double corner_x1=0, corner_y1=0;
double corner_x2=0, corner_y2=0;
int count_scr=0;
int count_gif=0;

extern int para;
extern double zoom;
extern double horizontal, vertical;
extern int WINDOWS_HEIGHT, WINDOWS_WIDTH;

void screenshot(){
  FILE *f;
  int x, y;
  int r, g, b;
  unsigned char *img = NULL;
  int filesize = 54 + 3*WINDOWS_WIDTH*WINDOWS_HEIGHT;
  char nome[50];

  GLfloat *array = malloc(3*WINDOWS_WIDTH*WINDOWS_HEIGHT*sizeof(GLfloat));
  glReadPixels(0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT, GL_RGB, GL_FLOAT, array);
  
  img = (unsigned char *)malloc(3*WINDOWS_WIDTH*WINDOWS_HEIGHT);
  memset(img,0,3*WINDOWS_WIDTH*WINDOWS_HEIGHT);
  for(int i=0; i<WINDOWS_WIDTH; i++){
    for(int j=0; j<WINDOWS_HEIGHT; j++){
      x=i; y=(WINDOWS_HEIGHT-1)-j;
      r=254*array[0+x*3+(WINDOWS_HEIGHT-1-y)*WINDOWS_WIDTH*3];
      g=254*array[1+x*3+(WINDOWS_HEIGHT-1-y)*WINDOWS_WIDTH*3];
      b=254*array[2+x*3+(WINDOWS_HEIGHT-1-y)*WINDOWS_WIDTH*3];
      if (r > 255) r=255;
      if (g > 255) g=255;
      if (b > 255) b=255;
      img[(x+y*WINDOWS_WIDTH)*3+2] = (unsigned char)(r);
      img[(x+y*WINDOWS_WIDTH)*3+1] = (unsigned char)(g);
      img[(x+y*WINDOWS_WIDTH)*3+0] = (unsigned char)(b);
    }
  }
  unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
  unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
  unsigned char bmppad[3] = {0,0,0};
  bmpfileheader[ 2] = (unsigned char)(filesize    );
  bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
  bmpfileheader[ 4] = (unsigned char)(filesize>>16);
  bmpfileheader[ 5] = (unsigned char)(filesize>>24);
  bmpinfoheader[ 4] = (unsigned char)(WINDOWS_WIDTH);
  bmpinfoheader[ 5] = (unsigned char)(WINDOWS_WIDTH>> 8);
  bmpinfoheader[ 6] = (unsigned char)(WINDOWS_WIDTH>>16);
  bmpinfoheader[ 7] = (unsigned char)(WINDOWS_WIDTH>>24);
  bmpinfoheader[ 8] = (unsigned char)(WINDOWS_HEIGHT);
  bmpinfoheader[ 9] = (unsigned char)(WINDOWS_HEIGHT>> 8);
  bmpinfoheader[10] = (unsigned char)(WINDOWS_HEIGHT>>16);
  bmpinfoheader[11] = (unsigned char)(WINDOWS_HEIGHT>>24);
  
  sprintf(nome, "screenshot_%04d.bmp", count_scr);
  f = fopen(nome,"wb");
  fwrite(bmpfileheader,1,14,f);
  fwrite(bmpinfoheader,1,40,f);
  for(int i=0; i<WINDOWS_HEIGHT; i++){
    fwrite(img+(WINDOWS_WIDTH*(WINDOWS_HEIGHT-i-1)*3),3,WINDOWS_WIDTH,f);
    fwrite(bmppad,1,(4-(WINDOWS_WIDTH*3)%4)%4,f);
  }
  free(img);
  fclose(f);
  count_scr++;
}

void animation_frame(){
  FILE *f;
  int x, y;
  int r, g, b;
  unsigned char *img = NULL;
  int filesize = 54 + 3*WINDOWS_WIDTH*WINDOWS_HEIGHT;
  char nome[50];

  GLfloat *array = malloc(3*WINDOWS_WIDTH*WINDOWS_HEIGHT*sizeof(GLfloat));
  glReadPixels(0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT, GL_RGB, GL_FLOAT, array);
  
  img = (unsigned char *)malloc(3*WINDOWS_WIDTH*WINDOWS_HEIGHT);
  memset(img,0,3*WINDOWS_WIDTH*WINDOWS_HEIGHT);
  for(int i=0; i<WINDOWS_WIDTH; i++){
    for(int j=0; j<WINDOWS_HEIGHT; j++){
      x=i; y=(WINDOWS_HEIGHT-1)-j;
      r=254*array[0+x*3+(WINDOWS_HEIGHT-1-y)*WINDOWS_WIDTH*3];
      g=254*array[1+x*3+(WINDOWS_HEIGHT-1-y)*WINDOWS_WIDTH*3];
      b=254*array[2+x*3+(WINDOWS_HEIGHT-1-y)*WINDOWS_WIDTH*3];
      if (r > 255) r=255;
      if (g > 255) g=255;
      if (b > 255) b=255;
      img[(x+y*WINDOWS_WIDTH)*3+2] = (unsigned char)(r);
      img[(x+y*WINDOWS_WIDTH)*3+1] = (unsigned char)(g);
      img[(x+y*WINDOWS_WIDTH)*3+0] = (unsigned char)(b);
    }
  }
  unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
  unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
  unsigned char bmppad[3] = {0,0,0};
  bmpfileheader[ 2] = (unsigned char)(filesize    );
  bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
  bmpfileheader[ 4] = (unsigned char)(filesize>>16);
  bmpfileheader[ 5] = (unsigned char)(filesize>>24);
  bmpinfoheader[ 4] = (unsigned char)(WINDOWS_WIDTH);
  bmpinfoheader[ 5] = (unsigned char)(WINDOWS_WIDTH>> 8);
  bmpinfoheader[ 6] = (unsigned char)(WINDOWS_WIDTH>>16);
  bmpinfoheader[ 7] = (unsigned char)(WINDOWS_WIDTH>>24);
  bmpinfoheader[ 8] = (unsigned char)(WINDOWS_HEIGHT);
  bmpinfoheader[ 9] = (unsigned char)(WINDOWS_HEIGHT>> 8);
  bmpinfoheader[10] = (unsigned char)(WINDOWS_HEIGHT>>16);
  bmpinfoheader[11] = (unsigned char)(WINDOWS_HEIGHT>>24);
  
  sprintf(nome, "anim_%04d.bmp", count_gif);
  f = fopen(nome,"wb");
  fwrite(bmpfileheader,1,14,f);
  fwrite(bmpinfoheader,1,40,f);
  for(int i=0; i<WINDOWS_HEIGHT; i++){
    fwrite(img+(WINDOWS_WIDTH*(WINDOWS_HEIGHT-i-1)*3),3,WINDOWS_WIDTH,f);
    fwrite(bmppad,1,(4-(WINDOWS_WIDTH*3)%4)%4,f);
  }
  free(img);
  fclose(f);
  count_gif++;
}


void mouseTranslate(){
  if(mouseLeftClick==1){
    if(flag==1){
      horizontal+=2*(mouseX-mouseXant)/WINDOWS_WIDTH/zoom;
      vertical+=2*(mouseYant-mouseY)/WINDOWS_HEIGHT/zoom;
    }
    flag=1;
    mouseXant=mouseX;
    mouseYant=mouseY;
  }
  else{
    flag=0;
  }
}

void cursorPositionCallback( GLFWwindow *window, double xPos, double yPos ){
  mouseX=xPos;
  mouseY=yPos;
}

void mouseButtonCallback( GLFWwindow *window, int button, int action, int mods ){
  GLFWcursor* cursor;

  if( button==0 && action==GLFW_PRESS ){
    mouseLeftClick=1;
    glfwSetCursor(window, glfwCreateStandardCursor(GLFW_HAND_CURSOR));
  }
  else if( button==0 && action==GLFW_RELEASE ){
    mouseLeftClick=0;
    glfwSetCursor(window, glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
  }

  /* if( button==1 && action==GLFW_PRESS ){ */
  /*   corner_x1=mouseX; */
  /*   corner_y1=mouseY; */
  /*   glfwSetCursor(window, glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR)); */
  /* } */
  /* else if( button==1 && action==GLFW_RELEASE ){ */
  /*   corner_x2=mouseX; */
  /*   corner_y2=mouseY; */
  /*   glfwSetCursor(window, glfwCreateStandardCursor(GLFW_ARROW_CURSOR)); */

  /*   horizontal+=-(corner_x2+corner_x1-WINDOWS_WIDTH)/WINDOWS_WIDTH/zoom1; */
  /*   vertical+=(corner_y2+corner_y1-WINDOWS_HEIGHT)/WINDOWS_HEIGHT/zoom2; */

  /*   zoom1+=(1+WINDOWS_WIDTH/fabs(corner_x2-corner_x1))*zoom1; */
  /*   zoom2+=(1+WINDOWS_HEIGHT/fabs(corner_y2-corner_y1))*zoom2; */
  /* } */
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

  if (key == 262 && (action == GLFW_REPEAT || GLFW_PRESS)){ horizontal-=0.05; } // Right arrow
  if (key == 263 && (action == GLFW_REPEAT || GLFW_PRESS)){ horizontal+=0.05; } // Left arrow
  if (key == 264 && (action == GLFW_REPEAT || GLFW_PRESS)){ vertical+=0.05; } // Down arrow
  if (key == 265 && (action == GLFW_REPEAT || GLFW_PRESS)){ vertical-=0.05; } // Up arrow

  if (key == 65 && action == GLFW_PRESS){ // Letra a - autoscale
    horizontal=0;
    vertical=0;
    zoom=0.8;
  }

  if (key == 45 && (action == GLFW_REPEAT || GLFW_PRESS)){ // Tecla -
    zoom-=zoomStep*zoom;
  }
  if (key == 61 && (action == GLFW_REPEAT || GLFW_PRESS)){ // Tecla +
    zoom+=zoomStep*zoom;
  }

  if (key == 83 && action == GLFW_PRESS){ // Letra s - screenshot
    screenshot();
  }
}

void scrollCallback( GLFWwindow *window, double xOffset, double yOffset ){
  zoom+=yOffset*zoomStep*zoom;
  glClear(GL_COLOR_BUFFER_BIT);
}
