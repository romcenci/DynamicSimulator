#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>

// Tamanho da tela
int SCREEN_WIDTH = 1000;
int SCREEN_HEIGHT = 600;

#define scale 1 // Escala plot tempo

int main(int argc, char **argv){
  int t=0, L=200, i, c, MODE=1, o;
  double x;

  ///////////////////////////////////////////// MENU
  o=2;
  while( (c = getopt(argc, argv, "lmhw")) != -1 ){
    switch (c){
    case 'l':
      L = atoi(argv[o]);
      break;
    case 'm':
      MODE = atoi(argv[o]);
      break;
    case 'h':
      SCREEN_HEIGHT = atoi(argv[o]);
      break;
    case 'w':
      SCREEN_WIDTH = atoi(argv[o]);
      break;
    }
    o+=2;
  }
  #include "SDLfunc.h" // Chamar esse header só aqui pq determina tamanho da janela
  /////////////////////////////////////////////

  init();
  bool quit = false; SDL_Event e;
  while( !quit ){
    while( SDL_PollEvent( &e ) != 0 ){
      if( e.type == SDL_QUIT ){
	quit = true;
      }
      else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE ){ // Se uma tecla for press e for espaço
	while(1){ // fica parado até que alguem break
	  usleep(100000); // espera 0.1s até ler a próxima vez
	  SDL_PollEvent( &e ); // le de novo
	  if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE){break;} // se a tecla for espaço, break
	  if(e.type == SDL_QUIT){quit=true; break;} // se quiser sair, saia
	}
	break;
      }
    }
    ////////////////////////////////////////////////////////////////////// MODE=1
    if(MODE==1){
      rect(0, (int)(scale*t)%(SCREEN_HEIGHT), SCREEN_WIDTH, 10, 0);

      for(i=0;i<L;i++){
	scanf("%lf\n", &x);
	put_pixel(SCREEN_WIDTH*x/5*0.8+SCREEN_WIDTH/10,(int)(scale*t)%(SCREEN_HEIGHT),0xFFFFFFFF);
      }

      SDL_UpdateWindowSurface( gWindow );
      t++;
    }
    //////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////// MODE=2
    else if(MODE==2){
      rect(0, (int)(scale*t)%(SCREEN_HEIGHT), SCREEN_WIDTH, 10, 0);

      for(i=0;i<L;i++){
	scanf("%lf\n", &x);
	rect(i*SCREEN_WIDTH/L, (int)(scale*t)%(SCREEN_HEIGHT), SCREEN_WIDTH/L, 1,(x+1.0)*120 + 14);
      }

      SDL_UpdateWindowSurface( gWindow );
      t++;
    }
    //////////////////////////////////////////////////////////////////////
  }

  closeS();
  return 0;
}
