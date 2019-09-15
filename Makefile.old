all: src/libbmp/libbmp.o src/colors.o src/draws.o src/modes.o src/other.o src/main

.PHONY: demos
dlist = $(wildcard demos/*.c)
dlistpp = $(wildcard demos/*.cpp)
demos: $(dlist:.c=.out) $(dlistpp:.cpp=.out)

#----- C
src/libbmp/libbmp.o: src/libbmp/libbmp.c
	gcc -c src/libbmp/libbmp.c -o src/libbmp/libbmp.o

src/colors.o: src/colors.c
	gcc -c src/colors.c -lm -lGL -lglfw -o src/colors.o

src/draws.o: src/draws.c
	gcc -c src/draws.c -lm -lGL -lglfw -o src/draws.o

src/modes.o: src/modes.c
	gcc -c src/modes.c -lm -lGL -lglfw -o src/modes.o

src/other.o: src/other.c
	gcc -c src/other.c -lm -lGL -lglfw -o src/other.o

src/main: src/main.c
	gcc src/main.c -lm -lGL -lglfw -o DynSim src/libbmp/libbmp.o src/colors.o src/draws.o src/modes.o src/other.o

#----- Demos
$(dlist:.c=.out): %.out: %.c $(wildcard %.h)
	gcc $< -O3 -lm -lfftw3 -o $(basename $<).out

$(dlistpp:.cpp=.out): %.out: %.cpp $(wildcard %.h)
	g++ $< -O3 -lm -lfftw3 -o $(basename $<).out

#----- Clean
clean:
	rm -f demos/*.out
	rm -f src/libbmp/libbmp.o
	rm -f src/colors.o
	rm -f src/draws.o
	rm -f src/modes.o
	rm -f src/other.o
	rm -f DynSim
