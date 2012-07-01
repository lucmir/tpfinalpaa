/************************************************************************
                            training.h
************************************************************************/

#ifndef TRAINING_H
#define TRAINING_H

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

#define NOT_FOUND   -900


class Training {
    // estruturas que armazenam a base de treino
    std::vector< std::vector<unsigned int> > userMovies;     // lista de filmes qualificados por cada usuario
    std::vector< std::vector<float> > userRatings;    // lista de qualificacoes por cada usuario

    // medias das qualificacoes por usuario
    std::vector<float> usersRatingsAvg;

    // medias das qualificacoes por filme
    std::vector<float> moviesRatingsAvg;

    // media de todas as qualificacoes
    float ratingsAvg;

public:
    // construtor
    Training();
    
    // destrutor
    ~Training();

    // funcoes sobre as estruturas
    void loadBase(char *baseFile);
    void printBase();
    void destroyBase();
    void loadUsersRatingsAvg(char *usersRatingsAvgFile);
    void loadMoviesRatingsAvg(char *moviesRatingsAvgFile);
    void updateUsersRatings();

    // funcoes de acesso
    float getRating(unsigned long int userIndex, unsigned int movieId);
    float getUserAvg(unsigned long int userId);
    float getMovieAvg(unsigned int movieId);
    float getRatingsAvg();
    void calculateRatingsAvg();
};


#endif 
