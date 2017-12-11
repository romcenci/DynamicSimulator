#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>

// Tamanho da tela
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;

#include "SDLfunc.h"

#define scale 1 // Escala plot tempo

int main(int argc, char **argv){
  int t=0, L=200, i, c, MODE=1, o;
  double x;

  ///////////////////////////////////////////// MENU
  o=2;
  while( (c = getopt(argc, argv, "lm")) != -1 ){
    switch (c){
    case 'l':
      L=atoi(argv[o]);
      break;
    case 'm':
      MODE=atoi(argv[o]);
      break;
    }
    o+=2;
  }
  /////////////////////////////////////////////
  
  init();
  bool quit = false; SDL_Event e;
  while( !quit ){
    while( SDL_PollEvent( &e ) != 0 ){
      if( e.type == SDL_QUIT ){
	quit = true;
      }
    }
    ////////////////////////////////////////////////////////////////////// MODE=1
    if(MODE==1){
      rect(0, (int)(scale*t)%(SCREEN_HEIGHT), SCREEN_WIDTH, 10, 0);

      for(i=0;i<L;i++){
	scanf("%lf\n", &x);
	put_pixel(SCREEN_WIDTH*x/5*0.8+100,(int)(scale*t)%(SCREEN_HEIGHT),0xFFFFFFFF);
      }

      SDL_UpdateWindowSurface( gWindow );
      t++;
    }
    //////////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////////////////////////////////// MODE=2
    else if(MODE==2){
      rect(0, (int)(scale*t)%(SCREEN_HEIGHT), SCREEN_WIDTH, 10, 0);

      for(i=0;i<L;i++){
	scanf("%*s %*d %lf\n", &x);
	rect(i*SCREEN_WIDTH/L, (int)(scale*t)%(SCREEN_HEIGHT), SCREEN_WIDTH/L, 1, x*200);
      }

      SDL_UpdateWindowSurface( gWindow );
      t++;
    }
    //////////////////////////////////////////////////////////////////////
  }
  
  closeS();
  return 0;
}
