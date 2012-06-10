CC = g++
LIBS = -lm -lpthread
CFLAGS = -Wall -c -fPIC -O3 -fomit-frame-pointer -ffast-math -DREENTRANT
PROGFLAGS = $(LIBS) -fPIC -O3 -fomit-frame-pointer -ffast-math -DREENTRANT

EXE = networkBuilder
OBJ = network.o networkBuilder.o

all: $(OBJ)
	$(CC) $(PROGFLAGS) $(OBJ) -o $(EXE)

network.o: network.h network.cpp
	$(CC) $(CFLAGS) network.cpp -o network.o

networkBuilder.o: networkBuilder.h networkBuilder.cpp
	$(CC) $(CFLAGS) networkBuilder.cpp -o networkBuilder.o


clean:
	rm .*.swp *~ $(OBJ) $(EXE)

backup:
	tar -cvzf backups/`date +%Y-%m-%d-%H.%M`.tar.gz *.h *.cpp Makefile 
