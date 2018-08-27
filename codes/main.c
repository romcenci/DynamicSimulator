#include "main.h"

int L=1000;
int MODE=0;
int WINDOWS_HEIGHT=400;
int WINDOWS_WIDTH=500;

int para;
double zoom1=1,zoom2=1;
double horizontal=0, vertical=0;

int NCOLORS=2;

double **allocateGRID(){
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

  return GRID;
}

int getopts(int argc, char *argv[]){
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
      NCOLORS=readColorFile(optarg);
      break;

    default:
      return 1;
    }
  }
}

int main(int argc, char *argv[]){
  GLFWwindow* window;

  pal=malloc(NCOLORS*sizeof(struct Color));
  HSV_Color HSV_pal[NCOLORS];
  RGB_Color RGB_pal[NCOLORS];

  pal[0].r=1.0; pal[1].r=0.0;
  pal[0].g=1.0; pal[1].g=0.0;
  pal[0].b=1.0; pal[1].b=0.0;

  getopts(argc, argv);

  //convertendo pal para a struct que criei, pra poder usar o conversor;
  for (int iii = 0; iii < NCOLORS; iii++){
     RGB_pal[iii].r = pal[iii].r;
     RGB_pal[iii].g = pal[iii].g;
     RGB_pal[iii].b = pal[iii].b;
  }
  //cria a palette em HSV
  // size_t ncolors = sizeof(RGB_pal)/sizeof(RGB_pal[0]);
  for(int iii = 0; iii < NCOLORS;iii++){
      printf("%f\t%f\t%f\n",RGB_pal[iii].r,RGB_pal[iii].g,RGB_pal[iii].b);
  }
  RGB2HSV(RGB_pal,HSV_pal, NCOLORS);
  HSV2RGB(RGB_pal,HSV_pal, NCOLORS);
  for(int iii = 0; iii < NCOLORS;iii++){
      printf("%f\t%f\t%f\n",RGB_pal[iii].r,RGB_pal[iii].g,RGB_pal[iii].b);
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

  glfwSetCursorPosCallback( window, cursorPositionCallback );
  glfwSetMouseButtonCallback( window, mouseButtonCallback );
  glfwSetKeyCallback( window, keyCallback );
  glfwSetScrollCallback( window, scrollCallback );

  double **GRID;
  GRID=allocateGRID();
  
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
      gridMode(tempo,GRID, HSV_pal);
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
  free(pal);
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
