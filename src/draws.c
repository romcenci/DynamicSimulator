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
  double rho =
      sqrt(pow(v2.y - v1.y, 2) + pow(v2.x - v1.x, 2) + pow(v2.z - v1.z, 2));
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

  if (COLOR_BOX == 0 || MODE == 1 || MODE == 3) {
    glPushMatrix();
    glTranslatef(-1, -1, 0);
    glColor3f(0, 0, 0);
    glRectf(0, 0, 2, 0.2);
    glRectf(0, 0, 0.2, 2);
    glRectf(1.8, 0, 2, 2);
    glRectf(0, 1.8, 2, 2);

    glColor3f(1, 1, 1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.2, 0.2);
    glVertex2f(0.2, 1.8);
    glVertex2f(1.8, 1.8);
    glVertex2f(1.8, 0.2);
    glEnd();
    glPopMatrix();
  } else {
    glPushMatrix();
    glTranslatef(-1, -1, 0);
    if (COLOR_BOX == 1) {
      glTranslatef(0, -0.075, 0);
      glColor3f(0, 0, 0);
      glRectf(0, 0, 2, 0.2);
      glRectf(0, 0, 0.2, 2);
      glRectf(1.8, 0, 2, 2);
      glRectf(0, 1.8, 2, 2.075);
      glColor3f(1, 1, 1);
      glBegin(GL_LINE_LOOP);
      glVertex2f(0.2, 0.2);
      glVertex2f(0.2, 1.8);
      glVertex2f(1.8, 1.8);
      glVertex2f(1.8, 0.2);
      glEnd();

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
    }

    else if (COLOR_BOX == 2) {
      glTranslatef(-0.075, 0, 0);
      glColor3f(0, 0, 0);
      glRectf(0, 0, 2, 0.2);
      glRectf(0, 0, 0.2, 2);
      glRectf(1.8, 0, 2.075, 2);
      glRectf(0, 1.8, 2, 2);
      glColor3f(1, 1, 1);
      glBegin(GL_LINE_LOOP);
      glVertex2f(0.2, 0.2);
      glVertex2f(0.2, 1.8);
      glVertex2f(1.8, 1.8);
      glVertex2f(1.8, 0.2);
      glEnd();

      glBegin(GL_QUADS);
      for (i = 0; i < NCOLORS - 1; i++) {
        glColor3f(pal[i].r, pal[i].g, pal[i].b);
        glVertex3f(1.85, (1.8 - 0.2) * i / (NCOLORS - 1) + 0.2, 0);
        glVertex3f(1.95, (1.8 - 0.2) * i / (NCOLORS - 1) + 0.2, 0);
        glColor3f(pal[i + 1].r, pal[i + 1].g, pal[i + 1].b);
        glVertex3f(1.95, (1.8 - 0.2) * (i + 1) / (NCOLORS - 1) + 0.2, 0);
        glVertex3f(1.85, (1.8 - 0.2) * (i + 1) / (NCOLORS - 1) + 0.2, 0);
      }
      glEnd();
      glColor3f(1, 1, 1);
      glBegin(GL_LINE_LOOP);
      glVertex2f(1.85, 0.2);
      glVertex2f(1.85, 1.8);
      glVertex2f(1.95, 1.8);
      glVertex2f(1.95, 0.2);
    }

    if (COLOR_BOX == 3) {
      glTranslatef(0, 0.075, 0);
      glColor3f(0, 0, 0);
      glRectf(0, -0.075, 2, 0.2);
      glRectf(0, 0, 0.2, 2);
      glRectf(1.8, 0, 2, 2);
      glRectf(0, 1.8, 2, 2);
      glColor3f(1, 1, 1);
      glBegin(GL_LINE_LOOP);
      glVertex2f(0.2, 0.2);
      glVertex2f(0.2, 1.8);
      glVertex2f(1.8, 1.8);
      glVertex2f(1.8, 0.2);
      glEnd();

      glBegin(GL_QUADS);
      for (i = 0; i < NCOLORS - 1; i++) {
        glColor3f(pal[i].r, pal[i].g, pal[i].b);
        glVertex3f((1.8 - 0.2) * i / (NCOLORS - 1) + 0.2, 0.15, 0);
        glVertex3f((1.8 - 0.2) * i / (NCOLORS - 1) + 0.2, 0.05, 0);
        glColor3f(pal[i + 1].r, pal[i + 1].g, pal[i + 1].b);
        glVertex3f((1.8 - 0.2) * (i + 1) / (NCOLORS - 1) + 0.2, 0.05, 0);
        glVertex3f((1.8 - 0.2) * (i + 1) / (NCOLORS - 1) + 0.2, 0.15, 0);
      }
      glEnd();
      glColor3f(1, 1, 1);
      glBegin(GL_LINE_LOOP);
      glVertex2f(0.2, 0.15);
      glVertex2f(1.8, 0.15);
      glVertex2f(1.8, 0.05);
      glVertex2f(0.2, 0.05);
    }

    else if (COLOR_BOX == 4) {
      glTranslatef(0.075, 0, 0);
      glColor3f(0, 0, 0);
      glRectf(0, 0, 2, 0.2);
      glRectf(-0.075, 0, 0.2, 2);
      glRectf(1.8, 0, 2, 2);
      glRectf(0, 1.8, 2, 2);
      glColor3f(1, 1, 1);
      glBegin(GL_LINE_LOOP);
      glVertex2f(0.2, 0.2);
      glVertex2f(0.2, 1.8);
      glVertex2f(1.8, 1.8);
      glVertex2f(1.8, 0.2);
      glEnd();

      glBegin(GL_QUADS);
      for (i = 0; i < NCOLORS - 1; i++) {
        glColor3f(pal[i].r, pal[i].g, pal[i].b);
        glVertex3f(0.15, (1.8 - 0.2) * i / (NCOLORS - 1) + 0.2, 0);
        glVertex3f(0.05, (1.8 - 0.2) * i / (NCOLORS - 1) + 0.2, 0);
        glColor3f(pal[i + 1].r, pal[i + 1].g, pal[i + 1].b);
        glVertex3f(0.05, (1.8 - 0.2) * (i + 1) / (NCOLORS - 1) + 0.2, 0);
        glVertex3f(0.15, (1.8 - 0.2) * (i + 1) / (NCOLORS - 1) + 0.2, 0);
      }
      glEnd();
      glColor3f(1, 1, 1);
      glBegin(GL_LINE_LOOP);
      glVertex2f(0.15, 0.2);
      glVertex2f(0.15, 1.8);
      glVertex2f(0.05, 1.8);
      glVertex2f(0.05, 0.2);
    }
    glEnd();
    glPopMatrix();
  }
}
