all: libbmp/libbmp.o DynSim

.PHONY: demos

dlist = $(wildcard demos/*.c)

libbmp/libbmp.o: libbmp/libbmp.c
	gcc -c libbmp/libbmp.c -o libbmp/libbmp.o

DynSim: DynSimOpenGL.c
	gcc DynSimOpenGL.c -lm -lGL -lglfw -o DynSim libbmp/libbmp.o

demos: $(dlist:.c=.out)

$(dlist:.c=.out): %.out: %.c $(wildcard %.h)
	gcc -O3 -lm -lfftw3 $< -o $(basename $<).out

clean:
	rm -f $(dlist:.c=.out)
