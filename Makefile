build:
	clang -g -Wall -std=c99 src/*.c -I SDL/inc/ -L SDL/lib/ -lSDL2 -lm -o renderer

run:
	./renderer

clean:
	rm renderer
	rm log.log

log:
	./renderer > log.log

read:
	tail -20 log.log