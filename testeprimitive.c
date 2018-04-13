#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
//Standard Libraries

const int WINDOWS_WIDTH = 640;
const int WINDOWS_HEIGHT = 480;

typedef struct
{
GLfloat x, y, z;
GLfloat r, g, b, a;
} Vertex;
void drawPoint(Vertex v1, GLfloat size);

void drawPointsDemo(int width, int height);


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

glEnable(GL_POINT_SMOOTH);
glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

while (!glfwWindowShouldClose(window))
{
float ratio;
int width, height;
glfwGetFramebufferSize(window, &width, &height);
ratio = (float) width / (float)height;
glViewport(0, 0, width, height);
glClear(GL_COLOR_BUFFER_BIT);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
//Orthographic Projection
glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

drawPointsDemo(width, height);
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
