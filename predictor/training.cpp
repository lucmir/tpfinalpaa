/************************************************************************
                            training.cpp
************************************************************************/

#include "training.h"


/* construtor */
Training::Training() {

}


/* destrutor */
Training::~Training() {

}

/* carrega a base (ou treino): 
 * a base deve estar formatada como uma linha por usuario
 * cada linha deve conter uma sequencia de movieId:rating
 * e deve utilizar espaco como separador    
*/
void Training::loadBase(char *baseFile) {
    std::ifstream file;
    std::string line, buff, movieId, rating;
    std::vector<unsigned int> movies_buff;
    std::vector<float> ratings_buff;
    unsigned long int totalRatings = 0;
    unsigned long int countRatings = 0;

    file.open(baseFile);
    if( !file.is_open() ) {
        std::cout << "\nError opening file!\n";
        std::exit(-1);
    }
    while(!file.eof()) {
        getline(file, line);
        if(file.eof())
            break;
        /* separa os campos */
        movies_buff.clear();
        ratings_buff.clear();
        std::stringstream ss(line);
        while(ss >> buff) {   
            std::stringstream ssBuff(buff);
            getline(ssBuff, movieId, ':');
            getline(ssBuff, rating, ':');
            movies_buff.push_back( atoi(movieId.c_str()) );
            ratings_buff.push_back( atof(rating.c_str()) );
            totalRatings += atoi(rating.c_str()); countRatings++;
        }
        this->userMovies.push_back(movies_buff);
        this->userRatings.push_back(ratings_buff);
    }    
    file.close();
    this->ratingsAvg = (float) totalRatings / (float) countRatings;
}


void Training::destroyBase() {
    unsigned long int i;
    for(i=0; i<userMovies.size(); i++) {
        userMovies[i].clear();
    }
    userMovies.clear();
    for(i=0; i<userRatings.size(); i++) {
        userRatings[i].clear();
    }
    userRatings.clear();
}


void Training::printBase() {
    unsigned long int i, j;
    for(i=0; i<userMovies.size(); i++) {
        for(j=0;j<userMovies[i].size();j++) {
            std::cout << " " << userMovies[i][j] << ":" << userRatings[i][j];
        }
        std::cout << std::endl;
    }
}


/* calcula media de qualificacoes para usuarios
 * e para items.
*/
void Training::calculateRatingsAvg() {
    unsigned int userRatingCount;
    std::vector<float> moviesRatingsCount;

    unsigned int maxItemIdx = 0;

    // calcula media de qualificacoes para usuarios
    for(unsigned int i=0; i<userRatings.size(); i++) {
        
        usersRatingsAvg.push_back(0.0);
        userRatingCount = 0;

        for(unsigned int j=0;j<userRatings[i].size();j++) {
            usersRatingsAvg[i] += userRatings[i][j];
            userRatingCount++;

            if(userMovies[i][j] > maxItemIdx) {
                maxItemIdx = userMovies[i][j];
            }
        }

        usersRatingsAvg[i] = (usersRatingsAvg[i] / (float) userRatingCount);
    }

    std::cout << maxItemIdx << "\n";

    // calcula media de qualificacoes para itens
    for(unsigned int i=0; i<maxItemIdx; i++) {
        moviesRatingsAvg.push_back(0.0);
        moviesRatingsCount.push_back(0.0);
    }

    for(unsigned int i=0; i<userRatings.size(); i++) {
        for(unsigned int j=0;j<userRatings[i].size();j++) {
            moviesRatingsAvg[userMovies[i][j]] += userRatings[i][j];
            moviesRatingsCount[userMovies[i][j]]++;
        }      
    }

    for(unsigned int i=0; i<maxItemIdx; i++) {
        moviesRatingsAvg[i] = moviesRatingsAvg[i] / moviesRatingsCount[i];
    }

}


/* carrega media de qualificacoes por usuario
 * arquivo deve estar formatado de forma que cada linha
 * represente um usuario e contenha somente o valor da media
 */
void Training::loadUsersRatingsAvg(char *usersRatingsAvgFile) {
    std::ifstream file;
    std::string line;
    float avg;

    file.open(usersRatingsAvgFile);
    if( !file.is_open() ) {
        std::cout << "\nError opening file!\n";
        std::exit(-1);
    }
    while(!file.eof()) {
        getline(file, line);
        if(file.eof())
            break;
        avg = atof(line.c_str());
        this->usersRatingsAvg.push_back(avg);
    }    
    file.close();
}

/* carrega media de qualificacoes por filme
 * arquivo deve estar formatado de forma que cada linha
 * represente um filme e contenha somente o valor da media
 */
void Training::loadMoviesRatingsAvg(char *moviesRatingsAvgFile) {
    std::ifstream file;
    std::string line;
    float avg;

    file.open(moviesRatingsAvgFile);
    if( !file.is_open() ) {
        std::cout << "\nError opening file!\n";
        std::exit(-1);
    }
    while(!file.eof()) {
        getline(file, line);
        if(file.eof())
            break;
        avg = atof(line.c_str());
        this->moviesRatingsAvg.push_back(avg);
    }    
    file.close();
}



/*  funcao que atualiza todas as qualificacoes da base em memoria para
 *  considerar a intencao do qualificador. Cada qualificacao e substituida
 *  pela variacao em relacao a media das qualificacoes do usuario. 
*/
void Training::updateUsersRatings() {
    float avg;
    std::map<unsigned int, float>::iterator itr;
    
    for(unsigned long int userIndex=0; userIndex<usersRatingsAvg.size(); userIndex++) {
        avg = this->usersRatingsAvg[userIndex];
        for(unsigned long int i=0; i<this->userRatings[userIndex].size(); i++) {
            this->userRatings[userIndex][i] -= avg;
        }
    } 
}


float Training::getUserAvg(unsigned long int userId) {
    return this->usersRatingsAvg[userId];
}

float Training::getMovieAvg(unsigned int movieId) {
    unsigned int movieIndex = movieId - 1;
    return this->moviesRatingsAvg[movieIndex];
}

float Training::getRatingsAvg() {
    return this->ratingsAvg;
}


float Training::getRating(unsigned long int userIndex, unsigned int movieId){
    if(userIndex < this->userMovies.size()) {
        for(unsigned long int i=0; i<this->userMovies[userIndex].size(); i++) {
            if(this->userMovies[userIndex][i] == movieId) {
                return this->userRatings[userIndex][i];
            }
        }
    }
    return NOT_FOUND;
}


