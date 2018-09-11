#include "main.h"

int L=1000;
int MODE=0;
int WINDOWS_HEIGHT=500;
int WINDOWS_WIDTH=600;

int para;
double zoom=0.8;
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
    return GRID;
  }

  else if(MODE==2){
    GRID=(double **)malloc(L*sizeof(double *));
    for (i = 0; i < L; ++i){
      GRID[i]=(double *)malloc(L*sizeof(double));
    }
    return GRID;
  }

  else if(MODE==3){
    GRID=(double **)malloc(L*sizeof(double *));
    for (i = 0; i < L; ++i){
      GRID[i]=(double *)malloc(3*sizeof(double));
    }
    return GRID;
  }

  else if(MODE==5){
    GRID=(double **)malloc(L*L*sizeof(double *));
    for (i = 0; i < L*L; ++i){
      GRID[i]=(double *)malloc(3*sizeof(double));
    }
    return GRID;
  }

  else{
    return 0;
  }
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

  pal[0].r=1.0; pal[1].r=0.0;
  pal[0].g=1.0; pal[1].g=0.0;
  pal[0].b=1.0; pal[1].b=0.0;

  getopts(argc, argv);

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
    ratio = (float)width / (float)height;
    glViewport(0, 0, width, height);

    glLoadIdentity();
    glPushMatrix();
    
    if(MODE==0 || MODE==1){
      glScalef(zoom,1,0);
      mouseTranslate();
      glTranslatef(horizontal, 0, 0);
    }
    if(MODE==2 || MODE==3 || MODE==5){
      glScalef(zoom,zoom,0);
      mouseTranslate();
      glTranslatef(horizontal, vertical, 0);
    }

    if(para==0){ tempo++; }

    if(MODE==0){
      gridMode(tempo, GRID);
    }
    else if(MODE==1){
      particleMode(tempo, GRID);
    }
    else if(MODE==2){
      grid2dMode(tempo, GRID);
    }
    else if(MODE==3){
      particle2dMode(tempo, GRID);
    }
    else if(MODE==4){
      SpectreMode(tempo);
    }
    else if(MODE==5){
      ArrowMode(tempo, GRID);
    }
    glPopMatrix();

    drawFrame();
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  free(GRID);
  free(pal);
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
