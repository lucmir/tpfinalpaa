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
            movies_buff.push_back( atoi(movieId.c_str()) );
            ratings_buff.push_back( atoi(rating.c_str()) );
        }
        userMovies.push_back(movies_buff);
        userRatings.push_back(ratings_buff);
    }    
    file.close();
}

/* carrega lista de usuarios do teste (lista de usuarios para o qual a rede sera gerada) */
void Network::loadTestUsersList(char *testUserListFile) {
    std::ifstream file;
    std::string line;
    unsigned long int userId;
    
    file.open(testUserListFile);
    if( !file.is_open() ) {
        std::cout << "\nError opening file!\n";
        std::exit(-1);
    }
    /* constroi lista de usuarios do teste */
    while(!file.eof()) {
        getline(file, line);
        if(file.eof())
            break;
        userId = atoi(line.c_str());
        this->testUsersList.push_back(userId);
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


/*TODO*/
/* fazer a funcao de atualizar ratings (base em memoria) */
/* fazer implementação da geração do vizinho e da impressão */
/* fazer sort externamente (de buildUserNeighb) para poder fazer em arquivos ou linhas separadas as 4 opções */
/* atualizar funcao de impressão */

/*  funcao que atualiza todas as qualificacoes da base em memoria para
 *  considerar a intencao do qualificador. Cada qualificacao e substituida
 *  pela variacao em relacao a media das qualificacoes do usuario. 
*/
void Network::updateUsersRatings(char *usersRatingsAvgFile) {
    std::ifstream file;
    std::string line;
    double avg;
    unsigned long int userIndex = 0;
    
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
        for(unsigned long int i=0; i<this->userRatings[userIndex].size(); i++) {
            this->userRatings[userIndex][i] -= avg;
        }
        userIndex++;
    } 
    file.close();
}


/* implementacao paralela da construcao da rede */
void Network::buildNetworkParallel(int numThreads, unsigned int minInters, unsigned int maxNeighbs, char *netOutFile) {
    pthread_t *threads;
    pthread_attr_t pthread_custom_attr;
    pthread_param *params;

    /* inicializa estruras das threads e mutex */
    threads = (pthread_t *) malloc(sizeof(pthread_t)*numThreads);
    params = (pthread_param *) malloc(sizeof(pthread_param)*numThreads);
    pthread_mutex_init(&MUTEX, NULL);
    pthread_attr_init(&pthread_custom_attr);

    /* executa construcao da vizinhanca em paralelo */
    for (int i=0; i<numThreads; i++)
    {
        params[i].threadId = i;
        params[i].numThreads = numThreads;
        params[i].This = this;
        params[i].netOutFile = netOutFile;
        params[i].minInters = minInters;
        params[i].maxNeighbs = maxNeighbs;
        pthread_create(&threads[i], NULL, &Network::buildNetwork, (void *)(params+i));
    }

    /* sincroniza finalizacao das threads */
    for (int i=0; i<numThreads; i++)
    {
        pthread_join(threads[i], NULL);
    }
	pthread_mutex_destroy(&MUTEX);
    free(threads);
    free(params);
}


void *Network::buildNetwork(void *arg) {
    pthread_param *param= (pthread_param *) arg;
    Network *network = param->This;
    unsigned long int userId, count;
    int vector_u[NUMBER_OF_MOVIES];
    int vector_v[NUMBER_OF_MOVIES];
    std::vector<Neighbor> userNeighb;
    userNeighb.resize(NUMBER_OF_USERS);

    /* abre 8 arquivos por thread */
    std::ofstream file[8];
    unsigned int fileNum = 0;
    for(unsigned int cmpFunc=1; cmpFunc<=4; cmpFunc++) {
        for(unsigned int type=1; type<=2; type++) {
            std::stringstream File;
            File << param->netOutFile << "." << cmpFunc << "." << type << "." << param->threadId;
            file[fileNum].open(File.str().c_str());
            if( !file[fileNum].is_open() ) {
                std::cout << "\nError opening file!\n";
                std::exit(-1);
            }
            fileNum++;
        }
    }

    /* constroi vizinhanca para cada usuario */
    for(unsigned int i=param->threadId; i<network->testUsersList.size(); i+=param->numThreads) {
        userId = network->testUsersList[i];
//      std::cout << "Construindo rede para usuario: "<< userId << std::endl;
        count = network->buildUserNeighb(userId, vector_u, vector_v, param->minInters, &userNeighb);
        if(count > 0) {
            network->printUserNeighb(file, &userNeighb, userId, count, param->maxNeighbs);
        }
    }

    for(unsigned int i=0; i<8; i++) {
        file[i].close();
    }
    pthread_exit(NULL);
    return NULL;
}


/* funcoes de comparacao utilizada na ordenacao dos vizinhos */
// TODO colocar dentro da classe como private depois
/* Ic x R */
bool neighbCmp1(Neighbor a, Neighbor b) {
    return ((a.cosinoIntens*a.represent) < (b.cosinoIntens*b.represent));
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


void Network::printUserNeighb(std::ofstream *file, std::vector<Neighbor> *userNeighb, unsigned long int userId, unsigned long int count, unsigned long int maxNeighbors) {
    unsigned long int i, j;

    /* ordena a vizinhanca pela funcao 1 e imprime */
    sort(userNeighb->begin(), userNeighb->begin()+count, neighbCmp1);
    for(i=0, j=count-1; (i < maxNeighbors && i < count); i++, j--) {
        file[0] << userId << " " << (*userNeighb)[i].userId << " " << (*userNeighb)[i].inters << " " << (*userNeighb)[i].cosinoIntens << " " <<  (*userNeighb)[i].pearsonIntens << " " << (*userNeighb)[i].represent << std::endl;
        file[1] << userId << " " << (*userNeighb)[j].userId << " " << (*userNeighb)[j].inters << " " << (*userNeighb)[j].cosinoIntens << " " <<  (*userNeighb)[j].pearsonIntens << " " << (*userNeighb)[j].represent << std::endl;
    }

    /* ordena a vizinhanca pela funcao 2 e imprime */
    sort(userNeighb->begin(), userNeighb->begin()+count, neighbCmp2);
    for(i=0, j=count-1; (i < maxNeighbors && i < count); i++, j--) {
        file[2] << userId << " " << (*userNeighb)[i].userId << " " << (*userNeighb)[i].inters << " " << (*userNeighb)[i].cosinoIntens << " " <<  (*userNeighb)[i].pearsonIntens << " " << (*userNeighb)[i].represent << std::endl;
        file[3] << userId << " " << (*userNeighb)[j].userId << " " << (*userNeighb)[j].inters << " " << (*userNeighb)[j].cosinoIntens << " " <<  (*userNeighb)[j].pearsonIntens << " " << (*userNeighb)[j].represent << std::endl;
    }

    /* ordena a vizinhanca pela funcao 3 e imprime */
    sort(userNeighb->begin(), userNeighb->begin()+count, neighbCmp3);
    for(i=0, j=count-1; (i < maxNeighbors && i < count); i++, j--) {
        file[4] << userId << " " << (*userNeighb)[i].userId << " " << (*userNeighb)[i].inters << " " << (*userNeighb)[i].cosinoIntens << " " <<  (*userNeighb)[i].pearsonIntens << " " << (*userNeighb)[i].represent << std::endl;
        file[5] << userId << " " << (*userNeighb)[j].userId << " " << (*userNeighb)[j].inters << " " << (*userNeighb)[j].cosinoIntens << " " <<  (*userNeighb)[j].pearsonIntens << " " << (*userNeighb)[j].represent << std::endl;
    }

    /* ordena a vizinhanca pela funcao 4 e imprime */
    sort(userNeighb->begin(), userNeighb->begin()+count, neighbCmp4);
    for(i=0, j=count-1; (i < maxNeighbors && i < count); i++, j--) {
        file[6] << userId << " " << (*userNeighb)[i].userId << " " << (*userNeighb)[i].inters << " " << (*userNeighb)[i].cosinoIntens << " " <<  (*userNeighb)[i].pearsonIntens << " " << (*userNeighb)[i].represent << std::endl;
        file[7] << userId << " " << (*userNeighb)[j].userId << " " << (*userNeighb)[j].inters << " " << (*userNeighb)[j].cosinoIntens << " " <<  (*userNeighb)[j].pearsonIntens << " " << (*userNeighb)[j].represent << std::endl;
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

