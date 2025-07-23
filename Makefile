CC=gcc

build:
	$(CC) -o bfi bfi.c

clean:
	rm bfi

run:
	$(CC) -o bfi bfi.c
	./bfi $(FILE)
	rm bfi

