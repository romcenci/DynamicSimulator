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
void drawLineSegment(Vertex v1, Vertex v2, GLfloat width) {
  glLineWidth(width);
  glBegin(GL_LINES);
    glColor4f(v1.r, v1.g, v1.b, v1.a);
  glVertex3f(v1.x, v1.y, v1.z);
  glColor4f(v2.r, v2.g, v2.b, v2.a);
  glVertex3f(v2.x, v2.y, v2.z);
  glEnd();
}
void DrawArrow(Vertex v1, Vertex v2, GLfloat width){
  int i;
  double theta = atan2((v2.y-v1.y),(v2.x-v1.x));
  double R=sqrt(pow(v2.y-v1.y,2)+pow(v2.x-v1.x,2));
  double rho=sqrt(pow(v2.y-v1.y,2)+pow(v2.x-v1.x,2)+pow(v2.z-v1.z,2));
  double headSize=0.5;

  glPushMatrix();
  glTranslatef(v1.x, v1.y, v1.z);
  glRotatef(theta/M_PI*180, 0.0, 0.0, 1.0f);

  glLineWidth(width);
  glBegin(GL_LINES);
  glColor4f(v2.r, v2.g, v2.b, v2.a);
  glVertex3f(0, 0, v2.z);
  glColor4f(v2.r, v2.g, v2.b, v2.a);
  glVertex3f(R, 0, v2.z);
  glEnd();

  glBegin(GL_TRIANGLES);
  glColor4f(v2.r, v2.g, v2.b, v2.a);
  glVertex3f(R-headSize*R, -0.7*headSize*rho, v2.z);
  glColor4f(v2.r, v2.g, v2.b, v2.a);
  glVertex3f(R, 0, v2.z);
  glColor4f(v2.r, v2.g, v2.b, v2.a);
  glVertex3f(R-headSize*R, 0.7*headSize*rho, v2.z);
  glEnd();
  glPopMatrix();
}
