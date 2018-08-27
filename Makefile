all: codes/libbmp/libbmp.o codes/colors.o codes/draws.o codes/modes.o codes/other.o codes/main

.PHONY: demos
dlist = $(wildcard demos/*.c)
dlistpp = $(wildcard demos/*.cpp)
demos: $(dlist:.c=.out) $(dlistpp:.cpp=.out)

#----- C
codes/libbmp/libbmp.o: codes/libbmp/libbmp.c
	gcc -c codes/libbmp/libbmp.c -o codes/libbmp/libbmp.o

codes/colors.o: codes/colors.c
	gcc -c codes/colors.c -lm -lGL -lglfw -o codes/colors.o

codes/draws.o: codes/draws.c
	gcc -c codes/draws.c -lm -lGL -lglfw -o codes/draws.o

codes/modes.o: codes/modes.c
	gcc -c codes/modes.c -lm -lGL -lglfw -o codes/modes.o

codes/other.o: codes/other.c
	gcc -c codes/other.c -lm -lGL -lglfw -o codes/other.o

codes/main: codes/main.c
	cd codes && gcc main.c -lm -lGL -lglfw -o ../DynSim libbmp/libbmp.o colors.o draws.o modes.o other.o

#----- Demos
$(dlist:.c=.out): %.out: %.c $(wildcard %.h)
	gcc $< -O3 -lm -lfftw3 -o $(basename $<).out

$(dlistpp:.cpp=.out): %.out: %.cpp $(wildcard %.h)
	g++ $< -O3 -lm -lfftw3 -o $(basename $<).out

#----- Clean
clean:
	rm -f demos/*.out
	rm -f codes/libbmp/libbmp.o
	rm -f codes/colors.o
	rm -f codes/draws.o
	rm -f codes/modes.o
	rm -f codes/other.o
	rm -f DynSim
