# DynamicSimulator

* Pre-requisitos
- Para baixar SDL2:
	"sudo apt-get install libsdl2-dev"

- Para compilar usa:
	"g++ animaSDL.cpp -lSDL2 -o animaSDL"
	"gcc animaSDL.c -lSDL2 -o animaSDL"
	
* Para rodar:
	Se a.out imprime vetor de tamanho L que evolui no tempo:
	"./a.out | ./DynSim -l 100 -m MODE"
	MODE pode ser escolhido como 1 ou 2, onde:
	- 1: Modo de posição variável;
	- 2: Modo de rede fixa para plotar cor.
