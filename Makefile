compile:
	gcc -Wall -std=c99 -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lm src/*.c -o build/renderer


run:
	make compile
	./build/renderer