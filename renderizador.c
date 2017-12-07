#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "simulation.c"

static GLfloat spin = 0.0;

void init(void){
  glClearColor (1.0, 1.0, 1.0, 1.0);
  glShadeModel (GL_FLAT);
  glClear(GL_COLOR_BUFFER_BIT);
}

void display(void){
  int i;
  double color;
  //glClear(GL_COLOR_BUFFER_BIT);
  glPointSize(3);
  glBegin(GL_POINTS);
  for(i=0;i<L;i++){
    color = (fi2[i]+1)/2;
    glColor3f(color, color, color);
    glVertex2f(i*90.0/L - 45.0, fmod(spin-45,90));
  }
  glEnd();

  glColor3f(1, 1, 1);
  glRectf(-50, spin-45, 50, spin-44);
  glutSwapBuffers();
}

void spinDisplay(void){
  spin=spin + 0.3;
  spin=fmod(spin,90);
  glutPostRedisplay();
}

void reshape(int w, int h){
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  //glLoadIdentity();
  glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
  //glMatrixMode(GL_MODELVIEW);
  //glLoadIdentity();
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
  /*Inicializando os vetores*/
  double *fi, *fi2,e;
  int i,j;
  fi = fftw_malloc(sizeof(double)*L);
  fi2 = fftw_malloc(sizeof(double)*L);
  vetores(fi);

  srand (1983479);

  for(i=0;i<L;i++)
    fi[i] = 2.0*rand()/RAND_MAX - 1.0;

  /*CRIA A JANELA*/
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (500, 500);
  glutInitWindowPosition (100, 100);
  glutCreateWindow (argv[0]);
  init();


  /*SIMULAÇÃO*/
  for(i=0;i<tprint;i++)
    update(fi);
   e = max(fi);
  for (i=0; i<L; i++)
    fi2[i] = fi[i]/e;

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMainLoop();
  return 0;
}
