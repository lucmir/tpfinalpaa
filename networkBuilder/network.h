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

#define NUMBER_OF_USERS    480189
#define NUMBER_OF_MOVIES    17770
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
    // estruturas que armazenam a base de treino
    std::vector< std::vector<int> > userMovies;     // lista de filmes qualificados por cada usuario
    std::vector< std::vector<char> > userRatings;    // lista de qualificacoes por cada usuario

    // medias das qualificacoes por usuario
    std::vector<double> usersRatingsAvg;

    // lista de usuarios que compoem o teste
    std::vector<unsigned long int> testUsersList;

    /* ponteiros para funcao */
   // Neighbor (Network::*calcIntensSimilarity)(unsigned long int user_u, unsigned long int user_v, int *vector_u, int *vector_v);

    /* estrutura que define os parametros para funcao da thread */
    struct pthread_param {
        int threadId;
        int numThreads;
        unsigned int minInters;
        unsigned int maxNeighbs;
        char *netOutFile;
        Network* This;
    };
    pthread_mutex_t MUTEX;

public:
    // construtor
    Network();
    
    // destrutor
    ~Network();

    void loadBase(char *baseFile);
    void printBase();
    void loadTestUsersList(char *testUserListFile);
    void updateUsersRatings(char *usersRatingsAvgFile);

    // funcoes para construcao da vizinhanca
    void buildNetworkParallel(int numThreads, unsigned int minInters, unsigned int maxNeighbs, char *netOutFile);
    static void *buildNetwork(void *arg);
    unsigned long int buildUserNeighb(unsigned long int user_u, int *vector_u, int *vector_v, unsigned int minInters, std::vector<Neighbor> *userNeighb);
    double calcRatingsAvg();
    void destroyBase();
    char getRating(unsigned long int userId, int movieId);
    void printUserNeighb(std::ofstream *file, std::vector<Neighbor> *userNeighb, unsigned long int userId, unsigned long int count, unsigned long int maxNeighbors);
};


#endif
