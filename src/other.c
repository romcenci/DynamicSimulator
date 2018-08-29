#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "libbmp/libbmp.h"
#include <GLFW/glfw3.h>

int flag;
int mouseLeftClick=0;
double zoomStep=0.2;
double mouseX, mouseY;
double mouseXant, mouseYant;
double corner_x1=0, corner_y1=0;
double corner_x2=0, corner_y2=0;

extern int para;
extern double zoom1, zoom2;
extern double horizontal, vertical;
extern int WINDOWS_HEIGHT, WINDOWS_WIDTH;

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
