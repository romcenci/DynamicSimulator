#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
//Standard Libraries


int main(void)
{
 GLFWwindow* window;
 if (!glfwInit()) //Inicia a bib, caso erro retorna
 exit(EXIT_FAILURE);
 window = glfwCreateWindow(640, 480, "Chapter 1: Simple GLFW Example", NULL, NULL);
 //(width,height,titulo,monitor(null = forma janela, caso contrario fullscreen),
 //share(janela que divide info null para nenhuma))
 if (!window)//inicia a janela, caso erro, termina a bib e retorna
 {
 glfwTerminate();
 exit(EXIT_FAILURE);
 }
 glfwMakeContextCurrent(window);//define qual janela vai ser alterada
 glfwSwapInterval(0); //ativa o VSync, reduz flickering, perceptível
 while (!glfwWindowShouldClose(window))//loop enquanto não fechar a janela
 {
 float ratio;
 int width, height;
 glfwGetFramebufferSize(window, &width, &height);
 //passa o endereço para armazenar os valores de width e height, que podem ser
 //modificados at will (resize) não define de fato o tamanho do buffer
 ratio = (float) width / (float) height;
 glViewport(0, 0, width, height);
 //(x,y = define a coordenada do canto esquerdo abaixo da janela de exibição(origem)
//width = largura, height = altura da tela) a função define a transformação de coordenadas
// de coordenadas normalizadas [0,1] para a coordenada em pixels;
 glClear(GL_COLOR_BUFFER_BIT);//limpa o buffer responsavel pelas cores da tela
 //só pode aparecer antes de glBegin e glEnd
 //onde definimos esse buffer? kkkk


//http://www.glprogramming.com/red/chapter03.html
//https://stackoverflow.com/questions/2571402/explain-the-usage-of-glortho
//explica melhor que eu as matrizes de transformação hahaha
 glMatrixMode(GL_PROJECTION);// matriz a ser modificada é a projetiva(como a camera ve)
 glLoadIdentity();// carrega matriz identidade
 glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);//
 glMatrixMode(GL_MODELVIEW);// matriz de como o objeto é visto
 glLoadIdentity();
 glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
 //(angle, x_fraction, y_fraction, z_fraction)
 //nas 3 ultimas coordenadas, coloca quanto de <<angle>> queremos rotacionar em cada eixo
 //pode ser negativo se queremos rotacionar para o outro lado
 //no nosso exemplo rotaciona tempo*50 graus as coordenadas abaixo
 glBegin(GL_TRIANGLES);//desenha 1 triangulo a cada 3 vertex
 glColor3f(1.f, 0.f, 0.f);//cor da proxima coordenada
 glVertex3f(-0.6f, -0.4f, 0.f);//coordenada
 glColor3f(0.f, 1.f, 0.f);
 glVertex3f(0.6f, -0.4f, 0.f);
 glColor3f(0.f, 0.f, 1.f);
 glVertex3f(0.f, 0.6f, 0.f);
 glEnd();// fecha o draw mode
 glfwSwapBuffers(window); //troca entre o front e o back buffer
 glfwPollEvents();// processa as mudanças na tela e os inputs
 }

 glfwDestroyWindow(window);//fecha a janela
 glfwTerminate();//fecha a biblioteca
 exit(EXIT_SUCCESS); //gg wp
}
