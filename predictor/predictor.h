/************************************************************************
                        predictor.h
************************************************************************/

#ifndef NETWORKBUILDER_H
#define NETWORKBUILDER_H

#include <vector>
#include <istream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <list>
#include <map>
#include <math.h>
#include <pthread.h>

#include "prediction.h"

using namespace std;

int main(int argc, char **argv);

/* recebe parametros:
    - trainingFile:         arquivo com base de treino (uma linha por usuario)
    - netSimFile:           arquivo com a rede de usuarios mais similares
    - testFile:             arquivo com a base de teste
    - numNeighbs:           numero maximo de vizinhos permitidos na rede
    - predictionsOutFile:   arquivo de saida que contera as predicoes
*/
short getArgs(int argc, char **argv, char **trainingFile, char **netSimFile, 
    char **testFile, char **predictionsOutFile);


void printUsage();


#endif
