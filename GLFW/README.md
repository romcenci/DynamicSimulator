Pasta para o GLFW



Para instalacao siga o tutorial abaixo:

http://www.opengl-tutorial.org/beginners-tutorials/tutorial-1-opening-a-window/

Seção building on linux até o último item antes do Qt Creator

e tambem: 

cmake make g++ libx11-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxrandr-dev libxext-dev libxcursor-dev libxinerama-dev libxi-dev

sudo apt-get install libglfw3-dev libglm-dev libglew-dev freeglut3-dev

os arquivos glad, linmath, etc da pasta devs têm que estar na mesma pasta do simulador;


teste.cpp segue o tutorial: 
http://www.opengl-tutorial.org/beginners-tutorials/tutorial-1-opening-a-window/

para rodar teste.cpp: gcc teste.cpp -lm -lglfw -lGLEW

teste.c segue o tutorial:
http://openglbook.com/chapter-1-getting-started.html

para rodar teste.c: gcc teste.c -lm -lGL -lglfw -lGLEW -lglut -lGLU

Vou tentar focar no segundo tutorial
