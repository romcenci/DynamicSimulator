#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

extern int WINDOWS_WIDTH;
extern int WINDOWS_HEIGHT;
extern int COLOR_BOX;
extern int NCOLORS;
extern int MODE;

struct Color {
  double r, g, b;
} * pal;

typedef struct {
  GLfloat x, y, z;
  GLfloat r, g, b, a;
} Vertex;

typedef struct { GLfloat x, y, z; } Data;

typedef struct { GLfloat a11, a12, a13, a21, a22, a23, a31, a32, a33; } Matrix;

void drawPoint(Vertex v1, GLfloat size) {
  glPointSize(size);
  glBegin(GL_POINTS);
  glColor4f(v1.r, v1.g, v1.b, v1.a);
  glVertex3f(v1.x, v1.y, v1.z);
  glEnd();
}

void DrawCircle(double x0, double y0, double r) {
  int i;

  glBegin(GL_POLYGON);
  for (i = 0; i <= 300; i++) {
    double angle = 2 * M_PI * i / 300;
    double x = r / WINDOWS_WIDTH * cos(angle) - x0;
    double y = r / WINDOWS_HEIGHT * sin(angle) - y0;
    glVertex2d(x, y);
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

void DrawArrow(Vertex v1, Vertex v2, GLfloat width) {
  int i;
  double theta = atan2((v2.y - v1.y), (v2.x - v1.x));
  double R = sqrt(pow(v2.y - v1.y, 2) + pow(v2.x - v1.x, 2));
  double rho = sqrt(pow(v2.y - v1.y, 2) + pow(v2.x - v1.x, 2) + pow(v2.z - v1.z, 2));
  double headSize = 0.5;

  glPushMatrix();
  glTranslatef(v1.x, v1.y, v1.z);
  glRotatef(theta / M_PI * 180, 0.0, 0.0, 1.0f);

  glLineWidth(width);
  glBegin(GL_LINES);
  glColor4f(v2.r, v2.g, v2.b, v2.a);
  glVertex3f(0, 0, v2.z);
  glColor4f(v2.r, v2.g, v2.b, v2.a);
  glVertex3f(R, 0, v2.z);
  glEnd();

  glBegin(GL_TRIANGLES);
  glColor4f(v2.r, v2.g, v2.b, v2.a);
  glVertex3f(R - headSize * R, -0.7 * headSize * rho, v2.z);
  glColor4f(v2.r, v2.g, v2.b, v2.a);
  glVertex3f(R, 0, v2.z);
  glColor4f(v2.r, v2.g, v2.b, v2.a);
  glVertex3f(R - headSize * R, 0.7 * headSize * rho, v2.z);
  glEnd();
  glPopMatrix();
}

void drawFrame() {
  int i;
  double offset_x, offset_y, menuSize=0.05;

  glTranslatef(0, -menuSize, 0);
  glPushMatrix();
  glTranslatef(-1, -1, 0);

  offset_x = ((COLOR_BOX+1)%2)*(COLOR_BOX-3.)*0.075;
  offset_y = (COLOR_BOX%2)*(COLOR_BOX-2.)*0.075;
  if(COLOR_BOX == 0 || MODE == 1 || MODE == 3){
    offset_x = 0;
    offset_y = 0;
  }

  // Black rectangles to make the borders
  glColor3f(0, 0, 0);
  glRectf(0, 0, 2, 0.2+offset_y);
  glRectf(0, 0, 0.2+offset_x, 2);
  glRectf(1.8+offset_x, 0, 2, 2);
  glRectf(0, 1.8+offset_y, 2, 2+menuSize);

  // White line in contour
  glColor3f(1, 1, 1);
  glBegin(GL_LINE_LOOP);
  glVertex2f(0.2+offset_x, 0.2+offset_y);
  glVertex2f(0.2+offset_x, 1.8+offset_y);
  glVertex2f(1.8+offset_x, 1.8+offset_y);
  glVertex2f(1.8+offset_x, 0.2+offset_y);
  glEnd();

  if(COLOR_BOX != 0 && MODE != 1 && MODE != 3){
    // Draw the colorbox
    glTranslatef(((COLOR_BOX+1)%2)*(-(offset_x/0.075-1.0)/2.*(2*0.825)-0.825), (COLOR_BOX%2)*(-(offset_y/0.075-1.0)/2.*(1.725-0.075)-1.725)+((COLOR_BOX+1)%2)*(-0.9), 0.0);

    double angle = ((COLOR_BOX+1)%2)*90.;
    glPushMatrix();

    glTranslatef(1.0, 1.9, 0.0);
    glRotatef(angle, 0., 0., 1.);
    glTranslatef(-1.0, -1.9, 0.0);

    glBegin(GL_QUADS);
    for (i = 0; i < NCOLORS - 1; i++) {
      glColor3f(pal[i].r, pal[i].g, pal[i].b);
      glVertex3f((1.8 - 0.2) * i / (NCOLORS - 1) + 0.2, 1.85, 0);
      glVertex3f((1.8 - 0.2) * i / (NCOLORS - 1) + 0.2, 1.95, 0);
      glColor3f(pal[i + 1].r, pal[i + 1].g, pal[i + 1].b);
      glVertex3f((1.8 - 0.2) * (i + 1) / (NCOLORS - 1) + 0.2, 1.95, 0);
      glVertex3f((1.8 - 0.2) * (i + 1) / (NCOLORS - 1) + 0.2, 1.85, 0);
    }
    glEnd();
    glColor3f(1, 1, 1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.2, 1.85);
    glVertex2f(1.8, 1.85);
    glVertex2f(1.8, 1.95);
    glVertex2f(0.2, 1.95);
    glEnd();
    glPopMatrix();
  }
  
  glPopMatrix();
}
