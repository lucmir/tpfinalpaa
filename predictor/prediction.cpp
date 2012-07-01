/************************************************************************
                            prediction.cpp
************************************************************************/

#include "prediction.h"


double DELTA;

// construtor
Prediction::Prediction() {

}

// destrutor
Prediction::~Prediction() {

}


void Prediction::makePredictions(char *testFileName, Network *network, Training *training,
	char *predictionsOutFile, Config *config) {
   
    std::ifstream testFile;
    std::ofstream outFile, infoFile;
    std::string line, buff;
    std::stringstream infoFileName, outFileName;
    unsigned long int userIndex;
    unsigned int movieId;
    float rightRating;
    PredValue pred;
    BinPredValue binPred;

    std::vector<double> *deltas = config->getDeltas();
    unsigned int numNeighbors = config->getNetSize();

    // imprime arquivo com informacoes
    infoFileName << predictionsOutFile << "." << numNeighbors << ".info";
    infoFile.open(infoFileName.str().c_str());
    if( !infoFile.is_open()){
        std::cout << "\nmakePredictions: Error opening info file!\n";
        std::exit(-1);
    }
    
    int testNumber = 1;
    infoFile << "[ 0 ] rightScore\n";
    if(config->getPredMode() == BINARY) { // binary
        for(unsigned int i=0; i<deltas->size(); i++) {
            double delta = (*deltas)[i];
            infoFile << "[ " << testNumber++ << " ] " << "binRightScore " << "delta=" << delta << "\n" ;
            infoFile << "[ " << testNumber++ << " ] " << "BINARY " << "delta=" << delta << " movieAvgPredFunction\n" ;
            infoFile << "[ " << testNumber++ << " ] " << "BINARY " << "delta=" << delta << " movieUsrPredFunction\n" ;
            infoFile << "[ " << testNumber++ << " ] " << "BINARY " << "delta=" << delta << " movieUsrAvgPredFunction\n" ;
        
            if(config->getSimFunction() == COSINO) { // coseno
                infoFile << "[ " << testNumber++ << " ] " << "BINARY " << " delta=" << delta << " simpleCosinoPredFunction\n" ;
            }
            else { // pearson
                infoFile << "[ " << testNumber++ << " ] " << "BINARY " << " delta=" << delta << " simplePearsonPredFunction\n" ;
            }
        }
    }
    else { // normal
        infoFile << "[ " << testNumber++ << " ] " << "NORMAL " << "movieAvgPredFunction\n" ;
        infoFile << "[ " << testNumber++ << " ] " << "NORMAL " << "movieUsrPredFunction\n" ;
        infoFile << "[ " << testNumber++ << " ] " << "NORMAL " << "movieUsrAvgPredFunction\n";

        if(config->getSimFunction() == COSINO) { // coseno
            infoFile << "[ " << testNumber++ << " ] " << "NORMAL " <<  " simpleCosinoPredFunction\n" ;
        }
        else { // pearson
            infoFile << "[ " << testNumber++ << " ] " << "NORMAL " <<  " simplePearsonPredFunction\n" ;
        }
    }

    infoFile.close();

    // abre testFile e outFile da thread
    testFile.open((const char *)testFileName);
    outFileName << predictionsOutFile << "." << numNeighbors;
    outFile.open(outFileName.str().c_str());
    if( !testFile.is_open() || !outFile.is_open() ){
        std::cout << "\nmakePredictions: Error opening files!\n";
        std::exit(-1);
    }

    // faz predicao para cada linha do teste
    while(!testFile.eof()) {
        getline(testFile, line);
        if(testFile.eof())
            break;
        // faz leitura da linha do teste
        std::stringstream ss(line);
        ss >> buff;
        movieId = atoi(buff.c_str());
        ss >> buff;
        userIndex = atoi(buff.c_str());
        ss >> buff;
        rightRating = atof(buff.c_str());

       /* esta linha diz ao algoritmo que so execute a predicao 
          para os usuarios que tem vizinhos suficientes */
        if( network->getSimNeighbors(userIndex)->size() < config->getPredRest()) {
           continue;
        }

        outFile << rightRating;
        if(config->getPredMode() == BINARY) {
            for(unsigned int i=0; i<deltas->size(); i++) {
                double delta = (*deltas)[i];
                // calcula valor predito para funcoes que utilizam a media
                double userAvg = training->getUserAvg(userIndex);
                if(rightRating < userAvg - delta) {
                    outFile << " " << NEGATIVE;
                }
                else if (rightRating > userAvg + delta) {
                    outFile << " " << POSITIVE;
                }
                else {
                    outFile << " " << NEUTRAL;
                }
                binPred = movieAvgPredFunctionBin(userIndex, movieId, training, delta);
                outFile << " " << binPred;
                binPred = usrAvgPredFunctionBin(userIndex, training, delta);
                outFile << " " << binPred;
                binPred = movieUsrAvgPredFunctionBin(userIndex, movieId, training, delta);
                outFile << " " << binPred;

                // calcula valor predito para cos ou pea
                if(config->getSimFunction() == COSINO) { // coseno
                    binPred = simpleCosinoPredFunctionBin(userIndex, movieId, network, training, delta, numNeighbors);
                    outFile << " " << binPred;
                } else{ // pearson
                    binPred = simplePearsonPredFunctionBin(userIndex, movieId, network, training, delta, numNeighbors);
                    outFile << " " << binPred;
                }
            }
        } else {
            // calcula valor predito para funcoes que utilizam a media
            pred = movieAvgPredFunction(movieId, training);
            outFile << " " << pred;
            pred = usrAvgPredFunction(userIndex, training);
            outFile << " " << pred;
            pred = movieUsrAvgPredFunction(userIndex, movieId, training);
            outFile << " " << pred;

            if(config->getSimFunction() == COSINO) { // coseno
                pred = simpleCosinoPredFunction(userIndex, movieId, network, training, numNeighbors);
                outFile << " " << pred;
            } else{ // pearson
                pred = simplePearsonPredFunction(userIndex, movieId, network, training, numNeighbors);
                outFile << " " << pred;
            }
        }
        outFile << "\n";
    }
   
    testFile.close();
    outFile.close();
}


/**
 *  Prediction functions
 */

/**
 *  General
 */

PredValue Prediction::movieAvgPredFunction(unsigned int movieId, Training *training) {
    double pred = training->getMovieAvg(movieId);
    if(pred < 1.0) {
        return 1.0;
    } else if(pred > 5.0) {
        return 5.0;
    }
    return pred;
}

PredValue Prediction::usrAvgPredFunction(unsigned long int userIndex, Training *training) {
    double pred = training->getUserAvg(userIndex);
    if(pred < 1.0) {
        return 1.0;
    } else if(pred > 5.0) {
        return 5.0;
    }
    return pred;
}

PredValue Prediction::movieUsrAvgPredFunction(unsigned long int userIndex, unsigned int movieId, Training *training) {
    double pred = training->getMovieAvg(movieId) - training->getRatingsAvg() + training->getUserAvg(userIndex);
    if(pred < 1.0) {
        return 1.0;
    } else if(pred > 5.0) {
        return 5.0;
    }
    return pred;
}

BinPredValue Prediction::movieAvgPredFunctionBin(unsigned long int userIndex, unsigned int movieId, Training *training, double delta) {
    double pred = training->getMovieAvg(movieId);
    if( pred < training->getUserAvg(userIndex) - delta ) {
        return NEGATIVE;
    } 
    else if( pred > training->getUserAvg(userIndex) + delta ) {
        return POSITIVE;
    }
    return NEUTRAL;
}

BinPredValue Prediction::usrAvgPredFunctionBin(unsigned long int userIndex, Training *training, double delta) {
    return NEUTRAL;
}

BinPredValue Prediction::movieUsrAvgPredFunctionBin(unsigned long int userIndex, unsigned int movieId, Training *training, double delta) {
    double pred = training->getMovieAvg(movieId) - training->getRatingsAvg();
    if( pred < - delta ) {
        return NEGATIVE;
    } 
    else if( pred > delta ) {
        return POSITIVE;
    }
    return NEUTRAL;
}


/**
 * Simple
 */

PredValue Prediction::simpleCosinoPredFunction(unsigned long int userIndex, unsigned int movieId, Network *network, Training *training, unsigned int numNeighbors) {
    double simsSum = 0.0;
    double prodSum = 0.0; 
    double rating, sim;
    unsigned long int neighbIndex, neighbId;
    std::vector<Neighbor> *simNeighbors = network->getSimNeighbors(userIndex);
    double pred;

    double lim = numNeighbors;
    for(unsigned long int i=0; i<simNeighbors->size(); i++) {
        if(i >= lim)
            break;
        /* obtem informacoes do vizinho */
        neighbId = (*simNeighbors)[i].userId;
        neighbIndex = neighbId;
        rating = training->getRating(neighbIndex, movieId);
        if(rating != NOT_FOUND){
            sim = (*simNeighbors)[i].cosinoIntens;
            /* calcula parte da predicao */
            prodSum += fabs(sim) * rating;
            simsSum += fabs(sim);
        }
    } 
    if(simsSum < 0.0 || simsSum > 0.0) {
        /* retorna o resultado predito */
        pred = (prodSum/simsSum);
        
    }
    else{
        pred = training->getMovieAvg(movieId) - training->getRatingsAvg() + training->getUserAvg(userIndex);
    }

    if(pred < 1.0) {
        return 1.0;
    } else if(pred > 5.0) {
        return 5.0;
    }
    return pred;
}


PredValue Prediction::simplePearsonPredFunction(unsigned long int userIndex, unsigned int movieId, Network *network, Training *training, unsigned int numNeighbors) {
    double simsSum = 0.0;
    double prodSum = 0.0; 
    double rating, sim;
    unsigned long int neighbIndex, neighbId;
    std::vector<Neighbor> *simNeighbors = network->getSimNeighbors(userIndex);
    double pred;

    double lim = numNeighbors;
    for(unsigned long int i=0; i<simNeighbors->size(); i++) {
        if(i >= lim)
            break;
        /* obtem informacoes do vizinho */
        neighbId = (*simNeighbors)[i].userId;
        neighbIndex = neighbId;
        rating = training->getRating(neighbIndex, movieId);
        if(rating != NOT_FOUND){
            sim = (*simNeighbors)[i].pearsonIntens;
            /* calcula parte da predicao */
            prodSum += fabs(sim) * rating;
            simsSum += fabs(sim);
        }
    } 
   
    if(simsSum < 0.0 || simsSum > 0.0) {
        /* retorna o resultado predito */
        pred = (prodSum/simsSum);
    }
    else{
        pred = training->getMovieAvg(movieId) - training->getRatingsAvg() + training->getUserAvg(userIndex);
    }

    if(pred < 1.0) {
        return 1.0;
    } else if(pred > 5.0) {
        return 5.0;
    }
    return pred;
}


BinPredValue Prediction::simpleCosinoPredFunctionBin(unsigned long int userIndex, unsigned int movieId, Network *network, Training *training, double delta, unsigned int numNeighbors) {
    double simsSum = 0.0;
    double prodSum = 0.0; 
    double rating, sim;
    unsigned long int neighbIndex, neighbId;
    std::vector<Neighbor> *simNeighbors = network->getSimNeighbors(userIndex);
    double pred;

    double lim = numNeighbors;
    for(unsigned long int i=0; i<simNeighbors->size(); i++) {
        if(i >= lim)
            break;
        /* obtem informacoes do vizinho */
        neighbId = (*simNeighbors)[i].userId;
        neighbIndex = neighbId;
        rating = training->getRating(neighbIndex, movieId);
        if(rating != NOT_FOUND){
            sim = (*simNeighbors)[i].cosinoIntens;
            /* calcula parte da predicao */
            prodSum += fabs(sim) * rating;
            simsSum += fabs(sim);
        }
    } 

    if(simsSum < 0.0 || simsSum > 0.0) {
        /* retorna o resultado predito */
        pred = (prodSum/simsSum);
        
    }
    else{
        pred = training->getMovieAvg(movieId) - training->getRatingsAvg();
    }

    if( pred < - delta ) {
        return NEGATIVE;
    } 
    else if( pred > delta ) {
        return POSITIVE;
    }
    return NEUTRAL;
}


BinPredValue Prediction::simplePearsonPredFunctionBin(unsigned long int userIndex, unsigned int movieId, Network *network, Training *training, double delta, unsigned int numNeighbors) {
    double simsSum = 0.0;
    double prodSum = 0.0; 
    double rating, sim;
    unsigned long int neighbIndex, neighbId;
    std::vector<Neighbor> *simNeighbors = network->getSimNeighbors(userIndex);
    double pred;

    double lim = numNeighbors;
    for(unsigned long int i=0; i<simNeighbors->size(); i++) {
        if(i >= lim)
            break;
        /* obtem informacoes do vizinho */
        neighbId = (*simNeighbors)[i].userId;
        neighbIndex = neighbId;
        rating = training->getRating(neighbIndex, movieId);
        if(rating != NOT_FOUND){
            sim = (*simNeighbors)[i].pearsonIntens;
            /* calcula parte da predicao */
            prodSum += fabs(sim) * rating;
            simsSum += fabs(sim);
        }
    } 


    if(simsSum < 0.0 || simsSum > 0.0) {
        /* retorna o resultado predito */
        pred = (prodSum/simsSum);
    }
    else{
        pred = training->getMovieAvg(movieId) - training->getRatingsAvg();
    }

    if( pred < - delta ) {
        return NEGATIVE;
    } 
    else if( pred > delta ) {
        return POSITIVE;
    }
    return NEUTRAL;
}

