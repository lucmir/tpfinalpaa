/***********************************************************************
                            config.h
************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

/** 
 * definicao de antes e valores que
 * servem de parametro para a execucao do
 * codigo.
*/

enum SimilarityFunction {
    COSINO,
    PEARSON
};

enum PredictionMode {
    NORMAL,
    BINARY
};


/**
 * configuracoes
 */

class Config {

    // funcao de similaridade
    int simFunction;

    // predicao em modo normal (valor de predicao) ou binaria
    int predMode;

    // para predicao binaria, define o delta necessario para ser + ou -
    std::vector<double> deltas;

    // tamanhos de rede
    int netSize;
    int loadNetSize;

    // numero necessario de vizinhos para que a predicao seja realizada
    unsigned int predRest;

public:
    Config() {
        this->simFunction = PEARSON;
        this->predMode = COSINO;
      
        double deltaValues[] = {0.0};
        this->deltas = std::vector<double>(deltaValues, deltaValues + sizeof(deltaValues) / sizeof(double));

        this->netSize = 2000;

        this->loadNetSize = 2000;
        this->predRest = 0;
    }

    int getSimFunction() {
        return this->simFunction;
    }

    int getPredMode() {
        return this->predMode;
    }

    std::vector<double> *getDeltas() {
        return &(this->deltas);
    }

    int getNetSize() {
        return this->netSize;
    }

    int getLoadNetSize() {
        return this->loadNetSize;
    }

    unsigned int getPredRest() {
        return this->predRest;
    }
    
};




#endif

