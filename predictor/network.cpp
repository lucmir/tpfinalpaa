/************************************************************************
                            network.h
************************************************************************/

#include "network.h"

/* construtor */
Network::Network(unsigned int numNeighbors){
    this->numNeighbors = numNeighbors;
}

/* destrutor */
Network::~Network() {
    this->destroy();
}


/*  Carrega a rede. Arquivos devem estar no seguinte formato:
 *  cada linha representa uma relacao:
 *      userIndex userId N Ic Ip R
 *  arquivo ordenado por ordem decrescente de 
 *  similaridade/dissimilaridade    
 * */
void Network::load(char *netSimFile) {
    std::ifstream simFile, disFile;
    std::string line;
    std::string userIndex, userId, n, ic, ip, r;
    Neighbor ngb;
    unsigned int count;
    std::string lastUserIndex;

    /* carrega vizinhos mais similares */
    simFile.open(netSimFile);
    if( !simFile.is_open() ) {
        std::cout << "\nError opening file!\n";
        std::exit(-1);
    }
    count = 0;
    lastUserIndex = "0";
    while(!simFile.eof()) {
        getline(simFile, line);
        if(simFile.eof())
            break;
        /* separa os campos */
        std::stringstream ss(line);
        ss>> userIndex, ss >> userId; ss >> n; ss >> ic; ss >> ip; ss >> r;

        if(userIndex != lastUserIndex) {
            count = 0;
            lastUserIndex = userIndex.c_str();
        }
        if(++count <= this->numNeighbors) {
            ngb.init(atoi(userId.c_str()), atoi(n.c_str()), atof(ic.c_str()), atof(ip.c_str()), atof(r.c_str()));
            this->netSim[ atoi(userIndex.c_str()) ].push_back(ngb);
        }
    }    
    simFile.close();
}


void Network::print() {
    std::map< unsigned long int, std::vector<Neighbor> >::iterator itr;
    unsigned long int userIndex;

    std::cout << "netSim:\n";
    for(itr=this->netSim.begin(); itr!=this->netSim.end(); itr++) {
        userIndex = (*itr).first;
        for(unsigned long int i=0; i!=(*itr).second.size(); i++) {
            std::cout << userIndex << " ";
            this->netSim[userIndex][i].print();
        }
    }
    std::cout << "netDis:\n";
    for(itr=this->netDis.begin(); itr!=this->netDis.end(); itr++) {
        userIndex = (*itr).first;
        for(unsigned long int i=0; i!=(*itr).second.size(); i++) {
            std::cout << userIndex << " ";
            this->netDis[userIndex][i].print();
        }
    }
}


void Network::destroy() {
    unsigned long int i;
    for(i=0; i<this->netSim.size(); i++) {
        this->netSim[i].clear();
    }
    this->netSim.clear();

    for(i=0; i<this->netDis.size(); i++) {
        this->netDis[i].clear();
    }
    this->netDis.clear();
}


/* funcoes de acesso */
std::vector<Neighbor> *Network::getSimNeighbors(unsigned long int userIndex) {
    return &this->netSim[userIndex];
}


std::vector<Neighbor> *Network::getDisNeighbors(unsigned long int userIndex) {
    return &this->netDis[userIndex];
}

unsigned int Network::getNumNeighbors(){
    return this->numNeighbors;
}
