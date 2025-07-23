CC=gcc
EXECNAME=bfi.out

build:
	$(CC) -o $(EXECNAME) bfi.c

clean:
	rm $(EXECNAME)

run:
	$(CC) -o $(EXECNAME) bfi.c
	./$(EXECNAME) $(FILE)
	rm $(EXECNAME)
