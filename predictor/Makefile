CC = g++
LIBS = -lm -lpthread
CFLAGS = -Wall -c -fPIC -O3 -fomit-frame-pointer -ffast-math -DREENTRANT
PROGFLAGS = $(LIBS) -fPIC -O3 -fomit-frame-pointer -ffast-math -DREENTRANT

EXE = predictor
OBJ = training.o predictor.o neighbor.o network.o prediction.o


all: $(OBJ)
	$(CC) $(PROGFLAGS) $(OBJ) -o $(EXE)


network.o: network.h network.cpp
	$(CC) $(CFLAGS) network.cpp -o network.o

prediction.o: prediction.h prediction.cpp config.h
	$(CC) $(CFLAGS) prediction.cpp -o prediction.o

predictor.o: predictor.h predictor.cpp
	$(CC) $(CFLAGS) predictor.cpp -o predictor.o

training.o: training.h training.cpp
	$(CC) $(CFLAGS) training.cpp -o training.o

neighbor.o: neighbor.h neighbor.cpp
	$(CC) $(CFLAGS) neighbor.cpp -o neighbor.o


clean:
	rm .*.swp *~ $(OBJ) $(EXE)

backup:
	tar -cvzf backups/`date +%Y-%m-%d-%H.%M`.tar.gz *.h *.cpp Makefile 
