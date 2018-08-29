#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include "libbmp/libbmp.h"
#define _USE_MATH_DEFINES

typedef struct {
    double h, s, v;
} HSV_Color;

typedef struct {
  double r, g, b;
} RGB_Color;

typedef struct{
  GLfloat x, y, z;
  GLfloat r, g, b, a;
} Vertex;

typedef struct{
  GLfloat x, y, z;
} Data;

struct Color{
  double r, g, b;
} *pal;

void drawPoint(Vertex v1, GLfloat size);
void drawPointsDemo(int width, int height);
void drawLineSegment(Vertex v1, Vertex v2, GLfloat width);
void DrawCircle(double x0, double y0, double r);
void drawGrid(GLfloat width, GLfloat height, GLfloat grid_width);

void gridMode(int tempo, double **GRID, HSV_Color *hsv_pal);
void particleMode(int tempo, double **GRID);
void grid2dMode(int tempo, double **GRID);
void particle2dMode(int tempo, double **GRID);
void SpectreMode();
void ArrowMode(int tempo, double **GRID);

void cursorPositionCallback( GLFWwindow *window, double xPos, double yPos );
void mouseButtonCallback( GLFWwindow *window, int button, int action, int mods );
void keyCallback( GLFWwindow *window, int key, int scancode, int action, int mods );
void scrollCallback( GLFWwindow *window, double xOffset, double yOffset );
void mouseTranslate();
void screenshot();

int readColorFile(char *optarg);
double min(double num1,double num2);
double max(double num1,double num2);
void RGB2HSV(RGB_Color *rgb_pal, HSV_Color *hsv_pal, int ncolors);
void HSV2RGB(RGB_Color *rgb_pal, HSV_Color *hsv_pal, int ncolors);
