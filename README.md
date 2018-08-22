# DynamicSimulator

* **Pre-requisitos:**
	* Instalar glfw3:
		- `sudo apt-get install libglfw3-dev`
	* Clonar o repositório com:
		- `git clone --recursive https://github.com/romcenci/DynamicSimulator.git`
	* Para compilar:
		- `make`
	
* **Opções:**
	* -l : Tamanho do vetor (quantidade de partículas)

	* -h (--height) : altura da tela
	
	* -w (--width) : largura da tela
	
	* -m (--mode) : Modo de plot
		- 0: (1d) Rede fixa (Ex: dampedGKS)
		<figure><img align=midle src="images/0.png" width="400"></figure>
		
		- 1: (1d) Posição variável (Granular. Ex: Quasicristal)		
		<figure><img align=midle src="images/1.png" width="400"></figure>
		
		- 2: (2d) Posição fixa (Ex: Ising 2d)		
		<figure><img align=midle src="images/2.png" width="400"></figure>
		
		- 3: (2d) Posição variável (Granular. Ex: Gás de bilhar)
		<figure><img align=midle src="images/3.png" width="400"></figure>
		
		- 5: (2d) Posição fixa (Flechas. Ex: Modelo de Heisenberg)
		<figure><img align=midle src="images/4.png" width="400"></figure>

* **Para rodar:**
	* Sintaxe:
		- `./a.out | ./DynSim -OPÇÕES`		
	* Exemplo:
		- `make demos`
		- `./demos/ogl_ex2_ising.c | ./DynSim -m 2 -l 300 --height 600 --width 800`
	
* **Interativo:**
	* <kbd>Spacebar</kbd> : Pausar
	* <kbd>&uparrow;</kbd> <kbd>&downarrow;</kbd> <kbd>&leftarrow;</kbd> <kbd>&rightarrow;</kbd>: Translação
	* <kbd>-</kbd> <kbd>+</kbd> : Zoom
	* <kbd>a</kbd> : Autoescala
	* <kbd>s</kbd> : Screenshot

# DynamicSimulator-octave

## Uso

- Compilar exemplos com `make demos`
- Rodar simulação com `./dynsim.m ./demos/ex2_ising.out`
