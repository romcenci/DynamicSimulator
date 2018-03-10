#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
//Standard Libraries
#define _USE_MATH_DEFINES // M_PI constant

const int WINDOWS_WIDTH = 880;
const int WINDOWS_HEIGHT = 660;

typedef struct
{
GLfloat x, y, z;
GLfloat r, g, b, a;
} Vertex;

typedef struct
{
GLfloat x, y, z;
} Data;

void drawPoint(Vertex v1, GLfloat size);
void drawPointsDemo(int width, int height);
void drawLineSegment(Vertex v1, Vertex v2, GLfloat width);
void drawGrid(GLfloat width, GLfloat height, GLfloat grid_width);
void DrawFrame();
void GksMode();



int main(void)
{
GLFWwindow* window;
if (!glfwInit())
exit(EXIT_FAILURE);
window = glfwCreateWindow(WINDOWS_WIDTH, WINDOWS_HEIGHT,
"Chapter 2: Primitive drawings", NULL, NULL);
if (!window){
glfwTerminate();
exit(EXIT_FAILURE);
}
glfwMakeContextCurrent(window);
// glfwSwapInterval(5);
glEnable(GL_POINT_SMOOTH);
glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

int tempo = 0;
while (!glfwWindowShouldClose(window))
{
  tempo++;
float ratio;
int width, height;
glfwGetFramebufferSize(window, &width, &height);
ratio = (float) width / (float)height;
glViewport(0, 0, width, height);
// glClear(GL_COLOR_BUFFER_BIT);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
//Orthographic Projection
// glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// drawGrid(5.0f, 1.0f, 0.1f);
DrawFrame();
GksMode(tempo);
glfwSwapBuffers(window);
glfwPollEvents();
}
glfwDestroyWindow(window);
glfwTerminate();
exit(EXIT_SUCCESS);
}

void drawPoint(Vertex v1, GLfloat size){
  glPointSize(size);
  glBegin(GL_POINTS);
  glColor4f(v1.r, v1.g, v1.b, v1.a);
  glVertex3f(v1.x, v1.y, v1.z);
  glEnd();
}

void drawPointsDemo(int width, int height){
GLfloat size=5.0f;
for(GLfloat x = 0.0f; x<=1.0f; x+=0.2f, size+=5)
{
Vertex v1 = {x, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
drawPoint(v1, size);
}
}

void drawLineSegment(Vertex v1, Vertex v2, GLfloat width) {
  glLineWidth(width);
  glBegin(GL_LINES);
  glVertex3f(v1.x, v1.y, v1.z);
  glColor4f(v2.r, v2.g, v2.b, v2.a);
  glVertex3f(v2.x, v2.y, v2.z);
  glEnd();
}

void drawGrid(GLfloat width, GLfloat height, GLfloat grid_width){
//horizontal lines
  for(float i=-height; i<height; i+=grid_width){
    Vertex v1 = {-width, i, 0.0f, 1.0f, 1.0f, 1.0f, 0.5f};
    Vertex v2 = {width, i, 0.0f, 1.0f, 1.0f, 1.0f, 0.5f};
    drawLineSegment(v1, v2,grid_width);
  }
//vertical lines
  for(float i=-width; i<width; i+=grid_width){
    Vertex v1 = {i, -height, 0.0f, 1.0f, 1.0f, 1.0f, 0.5f};
    Vertex v2 = {i, height, 0.0f, 1.0f, 1.0f, 1.0f, 0.5f};
    drawLineSegment(v1, v2,grid_width);
  }
}

void DrawFrame(){
Vertex v1 = {-0.95f, -0.95f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
Vertex v2 = {0.95f, -0.95f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
Vertex v3 = {0.95f, 0.95f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
Vertex v4 = {-0.95f, 0.95f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
  // drawLineSegment(v1, v2,3.0f);
  // drawLineSegment(v2, v3,3.0f);
  // drawLineSegment(v3, v4,3.0f);
  // drawLineSegment(v4, v1,3.0f);
}

void GksMode(int tempo){
int i;
float cor;
float yy;
Vertex v;
yy = (float) (-(tempo%600)+300.0f)/330.0f;
for(i = 0;i < 800;i++){
  scanf("%f\n", &cor);
  cor = (cor+1)/2.0;
  v.x = (i - 400.0)/440.0;
  v.y = yy;
  v.z = 0.0f;
  v.r = cor;
  v.g = cor;
  v.b = cor;
  v.a = 1.0f;
  drawPoint(v,1.0f);
  }
}
