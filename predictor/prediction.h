/************************************************************************
                            prediction.h
************************************************************************/

#ifndef PREDICTION_H
#define PREDICTION_H

#include <istream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <pthread.h>

#include "network.h"
#include "training.h"
#include "config.h"


/* valores de retorno para a predicao */
enum BinPredValue {
    NEGATIVE,
    NEUTRAL,
    POSITIVE
};

typedef double PredValue;

class Prediction {
    

public:
    // construtor
    Prediction();

    // destrutor
    ~Prediction();


    void makePredictions(char *testFileName, Network *network, Training *training,
        char *predictionsOutFile, Config *config);
    
    /**
     * Funcoes de predicao
     */

    /**
     * General
     */
    PredValue movieAvgPredFunction(unsigned int movieId, Training *training);

    PredValue usrAvgPredFunction(unsigned long int userIndex,
			Training *training);

    PredValue movieUsrAvgPredFunction(unsigned long int userIndex,
			unsigned int movieId, Training *training);

    BinPredValue movieAvgPredFunctionBin(unsigned long int userIndex,
			unsigned int movieId, Training *training, double delta);

    BinPredValue usrAvgPredFunctionBin(unsigned long int userIndex,
			Training *training, double delta);

    BinPredValue movieUsrAvgPredFunctionBin(unsigned long int userIndex,
			unsigned int movieId, Training *training, double delta);
    
    /**
     * Simple
     */
    PredValue simpleCosinoPredFunction(unsigned long int userIndex,
			unsigned int movieId, Network *network, Training *training,
			unsigned int numNeighbors);

    PredValue simplePearsonPredFunction(unsigned long int userIndex,
			unsigned int movieId, Network *network, Training *training,
			unsigned int numNeighbors);

    BinPredValue simpleCosinoPredFunctionBin(unsigned long int userIndex,
			unsigned int movieId, Network *network, Training *training,
			double delta, unsigned int numNeighbors);

    BinPredValue simplePearsonPredFunctionBin(unsigned long int userIndex,
			unsigned int movieId, Network *network, Training *training,
			double delta, unsigned int numNeighbors);

};

#endif

