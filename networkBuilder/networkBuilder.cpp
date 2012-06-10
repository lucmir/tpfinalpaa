/************************************************************************
                        networkBuilder.cpp
************************************************************************/

#include "networkBuilder.h"

/* funcao principal */
int main(int argc, char **argv) {   
    char *baseFile;
    char *testUserListFile;
    char *usersRatingsAvgFile;
    char *netOutFile;
    unsigned int minInters;
    unsigned int maxNeighbs;
    unsigned int numThreads;

    /* recebe parametros */
	if( !getArgs(argc, argv, &baseFile, &testUserListFile, &usersRatingsAvgFile, &netOutFile, &minInters, &maxNeighbs, &numThreads) )
		return 1;

    /* construcao da rede */
    cout << "\n\n\nPrograma Iniciado\n";
    Network network;
 
    cout << "\n\n\nCarregando a base...\n";
    network.loadBase(baseFile);

    cout << "\n\nCarregando lista de usarios do test...";
    network.loadTestUsersList(testUserListFile);

    cout << "\n\n\nAtualizando qualificacoes...\n";
    network.updateUsersRatings(usersRatingsAvgFile);

    cout << "\n\nConstruindo rede...";
    network.buildNetworkParallel(numThreads, minInters, maxNeighbs, netOutFile);

    /* merge files */
/*    string command = "cat ";
    for(int i=0; i<numThreads; i++) {
        command += netOutDri
    }
    cat 
*/
    return 0;
}

/* recebe parametros:
 * - baseFile = arquivo com o treino (ou base completa)
 * - testUserListFile = arquivo com a lista de usuarios contidos no test (indices dos usuarios)
 * - usersRatingsAvgFile = arquivo com a lista de medias de qualificacao por usuario
 * - netOutFile = arquivo de saida que contera as relacoes da rede 
 * - minInters = numero de intersecoes minimas para haver um relacionamento
 * - maxNeighbs = numero de vizinhos
 * - numThreads = numero de threads
*/
short getArgs(int argc, char **argv, char **baseFile, char **testUserListFile, char **usersRatingsAvgFile, char **netOutFile, unsigned int *minInters, unsigned int *maxNeighbs, unsigned int *numThreads) {
	int opt;
    *baseFile = NULL;
    *testUserListFile = NULL;
    *netOutFile = NULL;
    *usersRatingsAvgFile = NULL;
    *minInters = 0;
    *maxNeighbs = 0;
    *numThreads = 1;
	while( (opt = getopt(argc, argv, "hb:l:i:t:o:n:a:")) != -1 ) {
		switch(opt) {
            case 'h':
                printUsage();
                return 0;
                break;
            case 'b':
                *baseFile = optarg;
                break;
            case 'l':
                *testUserListFile = optarg;
                break;
            case 'a':
                *usersRatingsAvgFile = optarg;
                break;
            case 'i':
                *minInters = atoi(optarg);
                break;
            case 't': 
                *numThreads = atoi(optarg);
                break;    
            case 'n': 
                *maxNeighbs = atoi(optarg);
                break;    
            case 'o':
                *netOutFile = optarg;
                break;
			default: {
                printUsage();
				return 0;
			}
		}	
	}
    if(*baseFile == NULL || *testUserListFile == NULL || *usersRatingsAvgFile == NULL || *netOutFile == NULL || *minInters == 0 || *maxNeighbs == 0 || *numThreads <= 0 || *numThreads > 200) {
        printUsage();
        return 0;
    }
	return 1;
}	


void printUsage() {
    string args = "";
    args += "\t\t-b <baseFile> ";
    args += "-l <testUserListFile> ";
    args += "-a <usersRatingsAvgFile> ";
    args += "-o <netOutFile> ";
    args += "-i <minInters> ";
    args += "-n <maxNeighbs> ";
    args += "[-t <numThreads>]";
    args += " [-h]\n\n";
    cout << "\n\tArgumentos corretos:\n" << args;
}

