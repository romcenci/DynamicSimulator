#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//Standard Libraries
#define _USE_MATH_DEFINES // M_PI constant

const int WINDOWS_WIDTH = 700;
const int WINDOWS_HEIGHT = 700;

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
void drawLineDemo();
void drawTriangle(Vertex v1, Vertex v2, Vertex v3);
void drawTriangleDemo();
void linePlotDemo(float phase_shift);
void draw2DScatterPlot(const Data *data, int num_points);
void draw2DLineSegments(const Data *data, int num_points);
void gaussianDemo(float sigma);
void draw2DHeatMap(const Data *data, int num_points);




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

float sigma = 0.01f;
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

// phase_shift+=0.02f;
// linePlotDemo(phase_shift);
// drawTriangleDemo();
// drawLineDemo();
// drawPointsDemo(width, height);
drawGrid(5.0f, 1.0f, 0.1f);
sigma+=0.01f;
if(sigma>1.0f)
sigma=0.01;
gaussianDemo(sigma);
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

void drawLineDemo(){
  //draw a simple grid
  drawGrid(5.0f, 1.0f, 0.1f);
  //define the vertices and colors of the line segments
  Vertex v1 = {-5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.7f};
  Vertex v2 = {5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.7f};
  Vertex v3 = {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.7f};
  Vertex v4 = {0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.7f};
  //draw the line segments
  drawLineSegment(v1, v2, 10.0f);
  drawLineSegment(v3, v4, 10.0f);
}

void drawTriangle(Vertex v1, Vertex v2, Vertex v3){
  glBegin(GL_TRIANGLES);
  glColor4f(v1.r, v1.g, v1.b, v1.a);
  glVertex3f(v1.x, v1.y, v1.z);
  glColor4f(v2.r, v2.g, v2.b, v2.a);
  glVertex3f(v2.x, v2.y, v2.z);
  glColor4f(v3.r, v3.g, v3.b, v3.a);
  glVertex3f(v3.x, v3.y, v3.z);
  glEnd();
}

void drawTriangleDemo(){
//Triangle Demo
//draw a simple grid
drawGrid(5.0f, 1.0f, 0.1f);
Vertex v1 = {0.0f, 0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 0.6f};
Vertex v2 = {-1.0f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 0.6f};
Vertex v3 = {1.0f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 0.6f};
drawTriangle(v1, v2, v3);
}

  void draw2DScatterPlot (const Data *data, int num_points){
  Vertex v1 = {-10.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
  Vertex v2 = {10.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
  drawLineSegment(v1, v2, 2.0f);
  v1.x = 0.0f;
  v2.x = 0.0f;
  v1.y = -1.0f;
  v2.y = 1.0f;
  drawLineSegment(v1, v2, 2.0f);
  for(int i=0; i<num_points; i++){
    GLfloat x=data[i].x;
    GLfloat y=data[i].y;
    Vertex v={x, y, 0.0f, 1.0f, 1.0f, 1.0f, 0.5f};
    drawPoint(v, 8.0f);
  }
}

void draw2DLineSegments(const Data *data, int num_points){
  for(int i=0; i<num_points-1; i++){
    GLfloat x1=data[i].x;
    GLfloat y1=data[i].y;
    GLfloat x2=data[i+1].x;
    GLfloat y2=data[i+1].y;
    Vertex v1={x1, y1, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f};
    Vertex v2={x2, y2, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f};
    drawLineSegment(v1, v2, 4.0f);
  }
}

void linePlotDemo(float phase_shift){
  drawGrid(5.0f, 1.0f, 0.1f);
  GLfloat range = 10.0f;
  const int num_points = 200;
  Data *data=(Data*)malloc(sizeof(Data)*num_points);
  for(int i=0; i<num_points; i++){
    data[i].x=((GLfloat)i/num_points)*range-range/2.0f;
    data[i].y= 0.8f*cosf(data[i].x*3.14f+phase_shift);
  }
    draw2DLineSegments(data, num_points);
    draw2DScatterPlot(data, num_points);
  free(data);
}

void gaussianDemo(float sigma){
//construct a 1000x1000 grid
const int grid_x = 1000;
const int grid_y = 1000;
const int num_points = grid_x*grid_y;
Data *data=(Data*)malloc(sizeof(Data)*num_points);
int data_counter=0;
for(int x = -grid_x/2; x<grid_x/2; x+=1){
for(int y = -grid_y/2; y<grid_y/2; y+=1){
float x_data = 2.0f*x/grid_x;
float y_data = 2.0f*y/grid_y;
//compute the height z based on a
//2D Gaussian function.
float z_data = exp(-0.5f*(x_data*x_data)/(sigma*sigma)
-0.5f*(y_data*y_data)/(sigma*sigma))/
(sigma*sigma*2.0f*M_PI);
data[data_counter].x = x_data;
data[data_counter].y = y_data;
data[data_counter].z = z_data;
data_counter++;
}
}
//visualize the result using a 2D heat map
draw2DHeatMap(data, num_points);
free(data);
}

void draw2DHeatMap(const Data *data, int num_points){
//locate the maximum and minimum values in the dataset
  float max_value=-999.9f;
  float min_value=999.9f;
  for(int i=0; i<num_points; i++){
    const Data d = data[i];
    if(d.z > max_value){
      max_value = d.z;
    }
    if(d.z < min_value){
      min_value = d.z;
    }
  }
  const float halfmax = (max_value + min_value) / 2;
  //display the result
  glPointSize(1.0f);
  glBegin(GL_POINTS);
  for(int i = 0; i<num_points; i++){
    const Data d = data[i];
    float value = d.z;
    float b = 1.0f - value/halfmax;
    float r = value/halfmax - 1.0f;
    if(b < 0){
      b=0;
    }
    if(r < 0){
      r=0;
    }
    float g = 1.0f - b - r;
    glColor4f(r, g, b, 0.5f);
    glVertex3f(d.x, d.y, 0.0f);
  }
glEnd();
}
