build:
	gcc -Wall -std=c99 src/main.c -I SDL/inc/ -L ../SDL/lib/ -lSDL2 -o renderer

run:
	./renderer

clean:
	rm renderer