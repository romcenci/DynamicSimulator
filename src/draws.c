#include <math.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
typedef struct{
  GLfloat x, y, z;
  GLfloat r, g, b, a;
} Vertex;

typedef struct{
  GLfloat x, y, z;
} Data;

typedef struct{
    GLfloat a11, a12, a13, a21, a22, a23, a31, a32, a33;
} Matrix;


void drawPoint(Vertex v1, GLfloat size){
  glPointSize(size);
  glBegin(GL_POINTS);
  glColor4f(v1.r, v1.g, v1.b, v1.a);
  glVertex3f(v1.x, v1.y, v1.z);
  glEnd();
}

void DrawCircle(double x0, double y0, double r){
  int i;

  glBegin(GL_POLYGON);
  for(i=0; i <= 300; i++){
    double angle = 2 * M_PI * i / 300;
    double x = r/880*cos(angle)-x0;
    double y = r/660*sin(angle)-y0;
    glVertex2d(x,y);
  }
  glEnd();
}

void DrawArrow(Vertex v1, Vertex v2, GLfloat width){
  int i;
  double th=atan2((v2.y-v1.y),(v2.x-v1.x));
  double R=pow(v2.y-v1.y,2)+pow(v2.x-v1.x,2);
  double headSize=0.4;

  glPushMatrix();
  glTranslatef(v1.x, v1.y, v1.z);
  glRotatef(th/M_PI*180, 0.0, 0.0, 1.0f);

  glLineWidth(width);
  glBegin(GL_LINES);
  glVertex2d(0,0);
  glColor4f(v2.r, v2.g, v2.b, v2.a);
  glVertex2d(R-headSize*R,0);
  glEnd();

  glBegin(GL_POLYGON);
  glVertex2d(R-headSize*R,-headSize*R);
  glVertex2d(R,0);
  glVertex2d(R-headSize*R,headSize*R);
  glEnd();
  glPopMatrix();
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
}
Vertex VertexMatrixOp(Vertex v, Matrix M){
    Vertex v2 = {0.0f,0.0f,0.0f,v.r,v.g,v.b,v.a};

    v2.x = M.a11*v.x + M.a12*v.y + M.a13*v.z;
    v2.y = M.a21*v.x + M.a22*v.y + M.a23*v.z;
    v2.z = M.a31*v.x + M.a32*v.y + M.a33*v.z;
    return v2;
}

void DrawArrow2D(Vertex v1, Vertex v2, GLfloat width){
    double length = sqrt(powf(v2.x -v1.x,2)+powf(v2.y -v1.y,2)+powf(v2.z -v1.z,2));
    double length2 = sqrt(powf(v2.x -v1.x,2)+powf(v2.y -v1.y,2));
    double theta = atan2((v2.y-v1.y),(v2.x-v1.x));
    Vertex tp1 = {length2,0.0f,1.0f,v1.r,v1.g,v1.b,v1.a};
    Vertex tp2 = {0.8*length2,-0.01,1.0f,v1.r,v1.g,v1.b,v1.a};
    Vertex tp3 = {0.8*length2,0.01,1.0f,v1.r,v1.g,v1.b,v1.a};
    Matrix M = {cos(theta),-sin(theta),0,sin(theta),cos(theta),0,0,0,1};
    Matrix M2 = {1,0,v2.x,1,0,v2.y,0,0,1};

    tp1 = VertexMatrixOp(tp1,M);
    tp2 = VertexMatrixOp(tp2,M);
    // tp2 = VertexMatrixOp(tp2,M2);
    tp3 = VertexMatrixOp(tp3,M);
    tp1.x += v1.x;
    tp1.y += v1.y;
    tp2.x += v1.x;
    tp2.y += v1.y;
    tp3.x += v1.x;
    tp3.y += v1.y;
    tp3.z = tp2.z = tp1.z = v2.z;
    // tp3 = VertexMatrixOp(tp3,M2);
    // printf("%f\t%f\t%f\n%f\t%f\t%f\n%f\t%f\t%f\n",tp1.x,tp1.y,tp1.z,tp2.x,tp2.y,tp2.z,tp3.x,tp3.y,tp3.z );

    // tp1.z = tp2.z = tp3.z = 1.0f;
    glLineWidth(width);
    glBegin(GL_LINES);
    glColor4f(v2.r, v2.g, v2.b, v2.a);
    glVertex3f(v1.x, v1.y, v1.z);
    glColor4f(v2.r, v2.g, v2.b, v2.a);
    glVertex3f(v2.x, v2.y, v2.z);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor4f(v2.r, v2.g, v2.b, v2.a);
    glVertex3f(tp1.x, tp1.y, tp1.z);
    glColor4f(v2.r, v2.g, v2.b, v2.a);
    glVertex3f(tp2.x, tp2.y, tp2.z);
    glColor4f(v2.r, v2.g, v2.b, v2.a);
    glVertex3f(tp3.x, tp3.y, tp3.z);
    // // glColor4f(tp2.r, tp2.g, tp2.b, tp2.a);
    // glVertex3f(tp2.x, tp2.y, tp2.z);
    // // glColor4f(tp3.r, tp3.g, tp3.b, tp3.a);
    // glVertex3f(tp3.x, tp3.y, tp3.z);
    glEnd();
}
