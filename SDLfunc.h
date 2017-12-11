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

void closeS(){
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

void rect( double x, double y, double w, double h, int color){
  SDL_Rect p;
  p.x = x;
  p.y = y;
  p.w = w;
  p.h = h;

  SDL_FillRect(gScreenSurface, &p, SDL_MapRGB(gScreenSurface->format, color, color, color));
}

