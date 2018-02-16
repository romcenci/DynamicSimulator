# DynamicSimulator

* **Pre-requisitos:**
	* Para baixar SDL2:
		- "sudo apt-get install libsdl2-dev"
	
	* Para compilar usa:
		- "g++ animaSDL.cpp -lSDL2 -o animaSDL"
		- "gcc animaSDL.c -lSDL2 -o animaSDL"
	
* **Para rodar:**
	* Se a.out imprime vetor de tamanho 100 que evolui no tempo:
		- "./a.out | ./DynSim -OPÇÕES"
		
* **Outras opções**
	* -m : Modo de plot
		- 1: (1d) Posição variável
		- 2: (1d) Rede fixa
		- 3: (2d) Rede fixa
		
	* -l : (1d) Tamanho do vetor (quantidade de partículas)

	* -x : (2d) Tamanho da rede horizontal
	* -y : (2d) Tamanho da rede vertical
	
	* -h : Altura da janela
	* -w : Largura da janela
	

* **Testar o 2d:**
	* "./a.out | ./DynSimSDL -m 3 -x 400 -y 400 -w 800 -h 800"
