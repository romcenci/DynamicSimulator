all:

.PHONY: demos

dlist = $(wildcard demos/*.c)

demos: $(dlist:.c=.out)

$(dlist:.c=.out): %.out: %.c $(wildcard %.h)
	gcc -O3 -lm -lfftw3 $< -o $(basename $<).out

clean:
	rm -f $(dlist:.c=.out)
