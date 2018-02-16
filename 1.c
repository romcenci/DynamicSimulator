#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static GLfloat spin = 0.0;

void init(void){
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel (GL_FLAT);
  glClear(GL_COLOR_BUFFER_BIT);
}

void display(void){
  double i;
  //glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(1, 0, 0);
  glPointSize(1);
  glBegin(GL_POINTS);
  for(i=-50;i<50;i+=0.2){
    glVertex2f((float)rand()/RAND_MAX*100-50, fmod(spin-50,100));
  }
  glEnd();

  glColor3f(1, 1, 1);
  glRectf(-50, spin-50, 50, spin-49);
  glutSwapBuffers();
}

void spinDisplay(void){
  spin=spin + 0.1;
  spin=fmod(spin,100);
  glutPostRedisplay();
}

void reshape(int w, int h){
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void mouse(int button, int state, int x, int y) {
  switch (button) {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN)
      glutIdleFunc(spinDisplay);
    break;
  case GLUT_RIGHT_BUTTON:
    if (state == GLUT_DOWN)
      glutIdleFunc(NULL);
    break;
  default:
    break;
  }
}

int main(int argc, char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (500, 500); 
  glutInitWindowPosition (100, 100);
  glutCreateWindow (argv[0]);
  init();
  glutDisplayFunc(display); 
  glutReshapeFunc(reshape); 
  glutMouseFunc(mouse);
  glutMainLoop();
  return 0;
}

