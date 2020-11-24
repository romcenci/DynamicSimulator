all: src/colors.o src/draws.o src/modes.o src/other.o src/main

.PHONY: demos
dlist = $(wildcard demos/*.c)
dlistpp = $(wildcard demos/*.cpp)
demos: $(dlist:.c=.out) $(dlistpp:.cpp=.out)

#----- C
src/colors.o: src/colors.c
	gcc -c src/colors.c -lm -lGL -lglfw -o src/colors.o

src/draws.o: src/draws.c
	gcc -c src/draws.c -lm -lGL -lglfw -o src/draws.o

src/modes.o: src/modes.c
	gcc -c src/modes.c -lm -lGL -lglfw -o src/modes.o

src/other.o: src/other.c
	gcc -c src/other.c -lm -lGL -lglfw -o src/other.o

src/main: src/main.c
	gcc src/main.c -lGL -lglfw -o dynsim src/colors.o src/draws.o src/modes.o src/other.o -lm

#----- Install
PREFIX = /usr/local
.PHONY: install
install: dynsim
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/dynsim
	cp docs/dynsim.1.gz $(DESTDIR)/usr/share/man/man1/

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/dynsim
	rm -f $(DESTDIR)/usr/share/man/man1/dynsim.1.gz

#----- Demos
$(dlist:.c=.out): %.out: %.c $(wildcard %.h)
	gcc $< -O3 -lm -lfftw3 -o $(basename $<).out

$(dlistpp:.cpp=.out): %.out: %.cpp $(wildcard %.h)
	g++ $< -O3 -lm -lfftw3 -o $(basename $<).out

#----- Clean
clean:
	rm -f demos/*.out
	rm -f src/colors.o
	rm -f src/draws.o
	rm -f src/modes.o
	rm -f src/other.o
	rm -f dynsim
