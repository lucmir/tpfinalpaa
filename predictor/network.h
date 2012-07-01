/************************************************************************
                            network.h
************************************************************************/

#ifndef NETWORK_H
#define NETWORK_H

#include <istream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <pthread.h>

#include "neighbor.h"

typedef  std::map< unsigned long int, std::vector<Neighbor> > net_struct;
typedef  std::map< unsigned long int, std::vector<Neighbor> >::iterator net_itr;

/* classe que implementa a rede */
class Network {
    /* para cada usuario mantem uma lista de vizinhos ordenados pela similaridade 
     * userIndex -> {userId, userId, ...} 
     * */
    net_struct netSim;
    /* para cada usuario mantem uma lista de vizinhos ordenados pela dissimilaridade 
     * userIndex -> {userId, userId, ...} 
     * */
    net_struct netDis;

    unsigned int numNeighbors;

public:
    // construtor
    Network(unsigned int numNeighbors);
    
    // destrutor
    ~Network();

    void load(char *netSimFile);
    void print();
    void destroy();
    // funcoes de acesso
    std::vector<Neighbor> *getSimNeighbors(unsigned long int userIndex);
    std::vector<Neighbor> *getDisNeighbors(unsigned long int userIndex);
    unsigned int getNumNeighbors();
};


#endif
