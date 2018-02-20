# DynamicSimulator

* **Pre-requisitos:**
	* Para baixar SDL2:
		- "sudo apt-get install libsdl2-dev"
	
	* Para compilar usa:
		- "gcc DynSimSDL.c -lSDL2 -o DynSimSDL"
	
* **Para rodar:**
	* Se a.out imprime os dados:
		- "./a.out | ./DynSim -OPÇÕES"
		
* **Outras opções**
	* -m : Modo de plot
		- 1: (1d) Posição variável (Granular. Ex: Quasicristal)
		- 2: (1d) Rede fixa (Ex: dampedGKS)
		- 3: (2d) Posição variável (Granular. Ex: Gás de bilhar)
		- 4: (2d) Posição fixa (Ex: Ising 2d)
		
	* -l : Tamanho do vetor (quantidade de partículas)

	* -x :
		- (-m 3) Tamanho horizontal
		- (-m 4) Tamanho da rede horizontal
	* -y : 
		- (-m 3) Tamanho vertical
		- (-m 4) Tamanho da rede vertical
	
	* -h : Altura da janela
	* -w : Largura da janela
