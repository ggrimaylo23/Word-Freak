SRC = tree.c errors.c main.c
OBJ = tree.o errors.o main.o
PROG = wordfreak

$(PROG): $(OBJ)
	gcc $(OBJ) -o $(PROG)

$OBJ: $(SRC)
