CC=gcc
SRCNAME=bfi-jit.c
EXECNAME=bfi-jit.out

build:
	$(CC) -o $(EXECNAME) $(SRCNAME)

clean:
	rm $(EXECNAME)

run:
	$(CC) -o $(EXECNAME) $(SRCNAME)
	./$(EXECNAME) $(FILE)
	rm $(EXECNAME)
