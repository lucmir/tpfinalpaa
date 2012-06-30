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

#define COSENO_SIM          1
#define PEARSON_SIM         2
#define SIMPLE_ALGORITHM    1
#define NEW_ALGORITHM       2
#define INF                 100000


/* estrutura que define um vizinho na rede */
struct Neighbor {
    unsigned long int userId;
    unsigned int inters;
    double cosinoIntens;
    double pearsonIntens;
    double represent;
};


bool neighbCmp(Neighbor a, Neighbor b);


/* classe que implementa a rede */
class Network {
    
    std::vector< std::vector<int> > userMovies;     // lista de filmes qualificados por cada usuario
    std::vector< std::vector<char> > userRatings;   // lista de qualificacoes por cada usuario

    int maxMovieIndex;

    
public:
    // construtor
    Network();
    
    // destrutor
    ~Network();

    void loadBase(char *baseFile);
    
    void printBase();
    
    
    // funcoes para construcao da vizinhanca
    
    void buildNetwork(int minInters, char *netOutFile);
    
    unsigned long int buildUserNeighb(unsigned long int user_u, int *vector_u, int *vector_v, unsigned int minInters, std::vector<Neighbor> *userNeighb);
    
    void destroyBase();
    
    char getRating(unsigned long int userId, int movieId);
    
    void printUserNeighb(std::ofstream *file, std::vector<Neighbor> *userNeighb, unsigned long int userId, unsigned long int count);

};


#endif
