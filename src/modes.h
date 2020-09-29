#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

extern int L;
extern int NCOLORS;
extern int para;
extern int WINDOWS_WIDTH;
extern int WINDOWS_HEIGHT;
extern double zoom;
extern GLFWwindow *window;

typedef struct { double h, s, v; } HSV_Color;

typedef struct { double r, g, b; } RGB_Color;

extern struct Color { double r, g, b; } *pal;
/* struct Color { */
/*   double r, g, b; */
/* } * pal; */

typedef struct {
  GLfloat x, y, z;
  GLfloat r, g, b, a;
} Vertex;

typedef struct { GLfloat x, y, z; } Data;

void drawPoint(Vertex v1, GLfloat size);
void drawPointsDemo(int width, int height);
void drawLineSegment(Vertex v1, Vertex v2, GLfloat width);
void DrawCircle(double x0, double y0, double r);
void drawGrid(GLfloat width, GLfloat height, GLfloat grid_width);
void DrawFrame();
void DrawArrow(Vertex v1, Vertex v2, GLfloat width);
