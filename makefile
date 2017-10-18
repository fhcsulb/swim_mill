CC=cc
CCFLAGS = -pthread
LDFLAGS = -lpthread

all: swimmill fish pellet

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN)


swimmill: main.c
	$(CC) -o swimmill main.c $(CCFLAGS)

fish: fish.c
	$(CC) -o fish fish.c $(CCFLAGS)

pellet: pellet.c
	$(CC) -o pellet pellet.c $(CCFLAGS)
