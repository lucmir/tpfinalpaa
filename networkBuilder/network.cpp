/************************************************************************
                            network.h
************************************************************************/

#include "network.h"

/* construtor */
Network::Network(){

}

    
/* destrutor */
Network::~Network() {
    this->destroyBase();
}


/* carrega a base (ou treino): 
 * a base deve estar formatada como uma linha por usuario
 * cada linha deve conter uma sequencia de movieId:rating
 * e deve utilizar espaco como separador    
*/
void Network::loadBase(char *baseFile) {
    std::ifstream file;
    std::string line, buff, movieId, rating;
    std::vector<int> movies_buff;
    std::vector<char> ratings_buff;

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
            if(rating[rating.size()-1] == ' ') {
                rating = rating.substr(0, rating.size()-1);
            }
            int mId = atoi(movieId.c_str());
            if(mId > maxMovieIndex) {
                maxMovieIndex = mId;                
            }
            movies_buff.push_back( mId );
            ratings_buff.push_back( atoi(rating.c_str()) );
        }
        userMovies.push_back(movies_buff);
        userRatings.push_back(ratings_buff);
    }    
    file.close();
}


void Network::destroyBase() {
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


void Network::printBase() {
    unsigned long int i, j;
    for(i=0; i<userMovies.size(); i++) {
        for(j=0;j<userMovies[i].size();j++) {
            std::cout << " " << userMovies[i][j] << ":" << userRatings[i][j];
        }
        std::cout << std::endl;
    }
}


void Network::buildNetwork(int minInters, char *netOutFile) {  
    std::ofstream file;
    unsigned long int userId, count;
    int vector_u[this->maxMovieIndex];
    int vector_v[this->maxMovieIndex];
    std::vector<Neighbor> userNeighb;
    userNeighb.resize(userMovies.size());

    file.open(netOutFile);
    if( !file.is_open() ) {
        std::cout << "\nError opening file!\n";
        std::exit(-1);
    }

    /* constroi vizinhanca para cada usuario */
    for(userId=0; userId<userMovies.size(); userId++) {
//      std::cout << "Construindo rede para usuario: "<< userId << std::endl;
        count = this->buildUserNeighb(userId, vector_u, vector_v, minInters, &userNeighb);
        if(count > 0) {
            printUserNeighb(&file, &userNeighb, userId, count);
        }
    }

    file.close();
}


/* funcoes de comparacao utilizada na ordenacao dos vizinhos */
// TODO colocar dentro da classe como private depois
/* Ic x R */
bool neighbCmp1(Neighbor a, Neighbor b) {
    return ((a.cosinoIntens) < (b.cosinoIntens));
}   
/* Ip x R */
bool neighbCmp2(Neighbor a, Neighbor b) {
    return ((a.pearsonIntens*a.represent) < (b.pearsonIntens*b.represent));
}   
/* N x Ic x R */
bool neighbCmp3(Neighbor a, Neighbor b) {
    return ((a.inters*a.cosinoIntens*a.represent) < (b.inters*b.cosinoIntens*b.represent));
}   
/* n x ip x r */
bool neighbCmp4(Neighbor a, Neighbor b) {
    return ((a.inters*a.pearsonIntens*a.represent) < (b.inters*b.pearsonIntens*b.represent));
}


/*
    unsigned long int userId;
    unsigned int inters;
    double cosinoIntens;
    double pearsonIntens;
    double represent;
*/

/* obtem a vizinhanca para um determinado usuario */
unsigned long int Network::buildUserNeighb(unsigned long int user_u, int *vector_u, int *vector_v, unsigned int minInters, std::vector<Neighbor> *userNeighb) {
    unsigned long int count = 0;
    unsigned int inters;
    unsigned int u, v, count_u, count_v;
    double module_u, module_v, avg_u, avg_v, dotProduct; 
    double sum1, sum2, pearsonIntens;
    
    for(unsigned long int user_v=0; user_v<userMovies.size(); user_v++) {
        if(user_v != user_u) {
            /* faz o calculo de: inters, cosinoIntens, pearsonIntens, represent */
            module_u = module_v = avg_u = avg_v = dotProduct = 0.0;
            inters = 0;
            count_u = userMovies[user_u].size();
            count_v = userMovies[user_v].size();
            for(u=0,v=0; u < count_u && v < count_v;) {
                if(userMovies[user_u][u] > userMovies[user_v][v]) {
                    v++;
                }
                else if( userMovies[user_u][u] == userMovies[user_v][v] ) 
                {
                    vector_u[inters] = userRatings[user_u][u];
                    vector_v[inters] = userRatings[user_v][v];
                    dotProduct += vector_u[inters] * vector_v[inters];
                    module_u += pow(vector_u[inters], 2);
                    module_v += pow(vector_v[inters], 2);
                    avg_u += vector_u[inters];
                    avg_v += vector_v[inters];
                    u++;
                    v++;
                    inters++;
                }
                else {
                    u++;
                }
            }
            /* verifica restricao de intersecao minima */
            if(inters < minInters || inters <= 0 || module_u < 0.000001 || module_v < 0.000001) {
                continue;
            }
            
            /* calcula os termos para obter a pearsonIntens */
            avg_u /= inters;
            avg_v /= inters;
            sum1 = sum2 = pearsonIntens = 0.0;
            for(unsigned int j=0; j<inters; j++)
            {
                pearsonIntens += ((double)vector_u[j] - avg_u)*((double)vector_v[j] - avg_v);
                sum1 += pow((vector_u[j]-avg_u), 2);
                sum2 += pow((vector_v[j]-avg_v), 2);
            }

            Neighbor ngbr;
            ngbr.userId = user_v;
            ngbr.inters = inters;
            ngbr.cosinoIntens = (dotProduct / (module_u * module_v));
            ngbr.pearsonIntens = pearsonIntens;
            ngbr.represent = (inters / (double)(count_u + count_v - inters));
 
            /* inclui usuario na vizinhanca */
            (*userNeighb)[count++] = ngbr;
        }
    }
    return count;
}


void Network::printUserNeighb(std::ofstream *file, std::vector<Neighbor> *userNeighb, unsigned long int userId, unsigned long int count) {
    unsigned long int i, j;
    /* ordena a vizinhanca pela funcao 1 e imprime */
    sort(userNeighb->begin(), userNeighb->begin()+count, neighbCmp1);
    for(i=0, j=count-1; i < count; i++, j--) {
        file[0] << userId << " " << (*userNeighb)[i].userId << " " << (*userNeighb)[i].inters << " " << (*userNeighb)[i].cosinoIntens << " " <<  (*userNeighb)[i].pearsonIntens << " " << (*userNeighb)[i].represent << std::endl;
        //file[1] << userId << " " << (*userNeighb)[j].userId << " " << (*userNeighb)[j].inters << " " << (*userNeighb)[j].cosinoIntens << " " <<  (*userNeighb)[j].pearsonIntens << " " << (*userNeighb)[j].represent << std::endl;
    }
}



char Network::getRating(unsigned long int userId, int movieId) {
    for(unsigned int i=0; i<this->userMovies[userId].size(); i++) {
        if(this->userMovies[userId][i] == movieId) {
            return this->userRatings[userId][i];
        }
    }
    return 0;
}

