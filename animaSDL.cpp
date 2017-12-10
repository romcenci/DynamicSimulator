/* Anima a evolução usando a biblioteca SDL, tem que cuidar pois aqui a evolução ainda é síncrona */ 

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "rands.h"

//Tamanho do vetor do scanf
#define L 200

// Tamanho da tela
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 980;

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;

bool init(){
  //Initialization flag
  bool success = true;

  //Initialize SDL
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
    printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    success = false;
  }
  else{
    //Create window
    gWindow = SDL_CreateWindow( "SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( gWindow == NULL ){
      printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
      success = false;
    }
    else{
      //Get window surface
      gScreenSurface = SDL_GetWindowSurface( gWindow );
    }
  }

  return success;
}

void close(){
  //Destroy window
  SDL_DestroyWindow( gWindow );
  gWindow = NULL;

  //Quit SDL subsystems
  SDL_Quit();
}

void put_pixel ( int x, int y, Uint32 color ){
  SDL_Rect p;	
  p.x = x;
  p.y = y;
  p.w = p.h = 1;
  SDL_FillRect(gScreenSurface, &p, color);
}

void rect( double x, double y, double w, double h){
  SDL_Rect p;
  p.x = x;
  p.y = y;
  p.w = w;
  p.h = h;
  SDL_FillRect(gScreenSurface, &p, 0x0);
}

#define scale 1 // Escala plot tempo

int main(int argc, char **argv){
  int t=0, i;
  double x;
  
  init();
  bool quit = false; SDL_Event e;
  while( !quit ){
    while( SDL_PollEvent( &e ) != 0 ){ if( e.type == SDL_QUIT ){quit = true;} }
    //////////////////////////////////////////////////////////////////////
    rect(0, (int)(scale*t)%(SCREEN_HEIGHT), SCREEN_WIDTH, 10);

    for(i=0;i<L;i++){
      scanf("%lf\n", &x);
      put_pixel(SCREEN_WIDTH*x/5*0.9+100,(int)(scale*t)%(SCREEN_HEIGHT),0xFFFFFFFF);
    }

    SDL_UpdateWindowSurface( gWindow );
    t++;
    //////////////////////////////////////////////////////////////////////
  }
  
  close();
  return 0;
}
