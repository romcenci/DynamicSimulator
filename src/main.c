#include "main.h"

GLFWwindow *window;

int L = 200;
int MODE = 3;
int WINDOWS_HEIGHT = 500;
int WINDOWS_WIDTH = 500;
int NCOLORS = 2;
int COLOR_BOX = 0;

int gif_flag=0, gif_framerate=10;

int para;
double zoom = 0.8;
double horizontal = 0, vertical = 0;

int getopts(int argc, char *argv[]) {
  while (1) {
    static struct option long_options[] = {
        {"mode", required_argument, 0, 'm'},
        {"height", required_argument, 0, 'h'},
        {"width", required_argument, 0, 'w'},
        {"color", required_argument, 0, 'c'},
        {"colorbox", required_argument, 0, 'b'},
        {"gif", no_argument, 0, 'g'},
        {"gif_framerate", required_argument, 0, 'f'},
        {0, 0, 0, 0}};

    // getopt_long stores the option index here.
    int option_index = 0;

    int c =
        getopt_long(argc, argv, "l:m:h:w:c:b:gf:", long_options, &option_index);

    // Detect the end of the options.
    if (c == -1) {
      break;
    }

    switch (c) {
    case 0:
      break;

    case 'l':
      L = atoi(optarg);
      break;

    case 'm':
      MODE = atoi(optarg);
      break;

    case 'h':
      WINDOWS_HEIGHT = atoi(optarg);
      break;

    case 'w':
      WINDOWS_WIDTH = atoi(optarg);
      break;

    case 'c':
      NCOLORS = readColorFile(optarg);
      break;

    case 'b':
      if (strcmp(strdup(optarg), "top") == 0) {
        COLOR_BOX = 1;
      }
      if (strcmp(strdup(optarg), "right") == 0) {
        COLOR_BOX = 2;
      }
      if (strcmp(strdup(optarg), "bottom") == 0) {
        COLOR_BOX = 3;
      }
      if (strcmp(strdup(optarg), "left") == 0) {
        COLOR_BOX = 4;
      }
      break;

    case 'g':
      gif_flag = 1;
      break;

    case 'f':
      gif_framerate = atoi(optarg);
      break;

    default:
      break;
    }
  }
}

void configFile() {
  FILE *f1;
  if (f1 = fopen("dsconfig", "r")) {
    char arr[10];
    while (fscanf(f1, "%s:", arr) != EOF) {
      if (strstr(arr, "mode:")) {
        fscanf(f1, "%s", arr);
        MODE = atoi(arr);
      }
      if (strstr(arr, "L:")) {
        fscanf(f1, "%s", arr);
        L = atoi(arr);
      }
      if (strstr(arr, "width:")) {
        fscanf(f1, "%s", arr);
        WINDOWS_WIDTH = atoi(arr);
      }
      if (strstr(arr, "height:")) {
        fscanf(f1, "%s", arr);
        WINDOWS_HEIGHT = atoi(arr);
      }
      if (strstr(arr, "colorbox:")) {
        fscanf(f1, "%s", arr);
        if (strcmp(arr, "top") == 0) {
          COLOR_BOX = 1;
        }
        if (strcmp(arr, "right") == 0) {
          COLOR_BOX = 2;
        }
        if (strcmp(arr, "bottom") == 0) {
          COLOR_BOX = 3;
        }
        if (strcmp(arr, "left") == 0) {
          COLOR_BOX = 4;
        }
      }
    }
  }
}

int main(int argc, char *argv[]) {

  pal = malloc(NCOLORS * sizeof(struct Color));
  pal[1].r = 1.0;
  pal[0].r = 0.0;
  pal[1].g = 1.0;
  pal[0].g = 0.0;
  pal[1].b = 1.0;
  pal[0].b = 0.0;

  configFile();
  getopts(argc, argv);

  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }
  window = glfwCreateWindow(WINDOWS_WIDTH, WINDOWS_HEIGHT, "Dynamic Simulator",
                            NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);

  /* glEnable(GL_POINT_SMOOTH); */
  /* glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); */
  /* glEnable(GL_BLEND); */
  /* glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); */
  glEnable(GL_POINTS);
  
  glfwSetCursorPosCallback(window, cursorPositionCallback);
  glfwSetMouseButtonCallback(window, mouseButtonCallback);
  glfwSetKeyCallback(window, keyCallback);
  glfwSetScrollCallback(window, scrollCallback);

  int tempo = 0;
  float ratio;
  int width, height;

  ////////////////////////////////////////////////////////////////////

  if (MODE == 0) {
    int i;
    double offset_x, offset_y;
    if (COLOR_BOX == 1) {
      offset_y = -0.075;
    }
    if (COLOR_BOX == 2) {
      offset_x = -0.075;
    }
    if (COLOR_BOX == 3) {
      offset_y = 0.075;
    }
    if (COLOR_BOX == 4) {
      offset_x = 0.075;
    }
    double **Grid;
    Grid = (double **)malloc(600 * sizeof(double *));
    for (i = 0; i < 600; ++i) {
      Grid[i] = (double *)malloc(L * sizeof(double));
    }

    while (!glfwWindowShouldClose(window)) {
      glfwGetFramebufferSize(window, &WINDOWS_WIDTH, &WINDOWS_HEIGHT);
      ratio = (float)WINDOWS_WIDTH / (float)WINDOWS_HEIGHT;
      glViewport(0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT);

      glLoadIdentity();
      glPushMatrix();

      mouseTranslate();
      glTranslatef(horizontal + offset_x, offset_y, 0);
      glScalef(zoom, 0.8, 0);

      if (para == 0) {
        tempo++;
      }

      gridMode(tempo, Grid);
      glPopMatrix();

      drawFrame();

      glfwSwapBuffers(window);
      glfwPollEvents();

      if(gif_flag==1 && tempo%gif_framerate==0){
	animation_frame();
      }
    }
    if(gif_flag==1){
      system("convert -delay 5 -loop 0 *.bmp animation.gif 2> /dev/null; rm anim_*.bmp");
    }
    free(Grid);
  }

  ////////////////////////////////////////////////////////////////////

  else if (MODE == 1) {
    int i;
    double **Particles;
    Particles = (double **)malloc(600 * sizeof(double *));
    for (i = 0; i < 600; ++i) {
      Particles[i] = (double *)malloc(L * sizeof(double));
    }

    while (!glfwWindowShouldClose(window)) {
      glfwGetFramebufferSize(window, &WINDOWS_WIDTH, &WINDOWS_HEIGHT);
      ratio = (float)WINDOWS_WIDTH / (float)WINDOWS_HEIGHT;
      glViewport(0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT);

      glLoadIdentity();
      glPushMatrix();

      mouseTranslate();
      glTranslatef(horizontal, 0, 0);
      glScalef(zoom, 0.8, 0);

      if (para == 0) {
        tempo++;
      }

      particleMode(tempo, Particles);
      glPopMatrix();

      drawFrame();

      glfwSwapBuffers(window);
      glfwPollEvents();
      if(gif_flag==1 && tempo%gif_framerate==0){
	animation_frame();
      }
    }
    if(gif_flag==1){
      system("convert -delay 5 -loop 0 *.bmp animation.gif 2> /dev/null; rm anim_*.bmp");
    }
    free(Particles);
  }

  ////////////////////////////////////////////////////////////////////

  else if (MODE == 2) {
    int i;
    double offset_x, offset_y;
    if (COLOR_BOX == 1) {
      offset_y = -0.075;
    }
    if (COLOR_BOX == 2) {
      offset_x = -0.075;
    }
    if (COLOR_BOX == 3) {
      offset_y = 0.075;
    }
    if (COLOR_BOX == 4) {
      offset_x = 0.075;
    }
    double **Grid;
    Grid = (double **)malloc(L * sizeof(double *));
    for (i = 0; i < L; ++i) {
      Grid[i] = (double *)malloc(L * sizeof(double));
    }
    while (!glfwWindowShouldClose(window)) {
      glfwGetFramebufferSize(window, &WINDOWS_WIDTH, &WINDOWS_HEIGHT);
      ratio = (float)WINDOWS_WIDTH / (float)WINDOWS_HEIGHT;
      glViewport(0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT);

      glLoadIdentity();
      glPushMatrix();

      mouseTranslate();
      glTranslatef(horizontal + offset_x, vertical + offset_y, 0);
      glScalef(zoom, zoom, 0);

      if (para == 0) {
        tempo++;
      }

      grid2dMode(tempo, Grid);
      glPopMatrix();

      drawFrame();

      glfwSwapBuffers(window);
      glfwPollEvents();

      if(gif_flag==1 && tempo%gif_framerate==0){
	animation_frame();
      }
    }
    if(gif_flag==1){
      system("convert -delay 5 -loop 0 *.bmp animation.gif 2> /dev/null; rm anim_*.bmp");
    }
    free(Grid);
  }

  ////////////////////////////////////////////////////////////////////

  else if (MODE == 3) {
    int i;
    double **Particles = (double **)malloc(L * sizeof(double *));
    for (i = 0; i < L; ++i) {
      Particles[i] = (double *)malloc(3 * sizeof(double));
    }
    while (!glfwWindowShouldClose(window)) {
      glfwGetFramebufferSize(window, &WINDOWS_WIDTH, &WINDOWS_HEIGHT);
      ratio = (float)WINDOWS_WIDTH / (float)WINDOWS_HEIGHT;
      glViewport(0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT);

      glLoadIdentity();
      glPushMatrix();

      mouseTranslate();
      glTranslatef(horizontal, vertical, 0);
      glScalef(zoom, zoom, 0);

      if (para == 0) {
        tempo++;
      }

      particle2dMode(tempo, Particles);
      glPopMatrix();

      drawFrame();

      glfwSwapBuffers(window);
      glfwPollEvents();
      if(gif_flag==1 && tempo%gif_framerate==0){
	animation_frame();
      }
    }
    if(gif_flag==1){
      system("convert -delay 5 -loop 0 *.bmp animation.gif 2> /dev/null; rm anim_*.bmp");
    }
    free(Particles);
  }

  ////////////////////////////////////////////////////////////////////

  else if (MODE == 4) {
    double **Grid;
    while (!glfwWindowShouldClose(window)) {
      glfwGetFramebufferSize(window, &WINDOWS_WIDTH, &WINDOWS_HEIGHT);
      ratio = (float)WINDOWS_WIDTH / (float)WINDOWS_HEIGHT;
      glViewport(0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT);

      glLoadIdentity();
      glPushMatrix();

      if (para == 0) {
        tempo++;
      }

      SpectreMode(tempo, Grid);
      glPopMatrix();

      drawFrame();

      glfwSwapBuffers(window);
      glfwPollEvents();
      if(gif_flag==1 && tempo%gif_framerate==0){
	animation_frame();
      }
    }
    if(gif_flag==1){
      system("convert -delay 5 -loop 0 *.bmp animation.gif 2> /dev/null; rm anim_*.bmp");
    }

    free(Grid);
  }

  ////////////////////////////////////////////////////////////////////

  else if (MODE == 5) {
    int i;
    double **Grid;
    Grid = (double **)malloc(L * L * sizeof(double *));
    for (i = 0; i < L * L; ++i) {
      Grid[i] = (double *)malloc(3 * sizeof(double));
    }
    while (!glfwWindowShouldClose(window)) {
      glfwGetFramebufferSize(window, &WINDOWS_WIDTH, &WINDOWS_HEIGHT);
      ratio = (float)WINDOWS_WIDTH / (float)WINDOWS_HEIGHT;
      glViewport(0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT);

      glLoadIdentity();
      glPushMatrix();

      mouseTranslate();
      glTranslatef(horizontal, vertical, 0);
      glScalef(zoom, zoom, 0);

      if (para == 0) {
        tempo++;
      }

      ArrowMode(tempo, Grid);
      glPopMatrix();

      drawFrame();

      glfwSwapBuffers(window);
      glfwPollEvents();
      if(gif_flag==1 && tempo%gif_framerate==0){
	animation_frame();
      }
    }
    if(gif_flag==1){
      system("convert -delay 5 -loop 0 *.bmp animation.gif 2> /dev/null; rm anim_*.bmp");
    }

    free(Grid);
  }

  ////////////////////////////////////////////////////////////////////

  free(pal);
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
