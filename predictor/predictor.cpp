/************************************************************************
                        predictor.cpp
************************************************************************/

#include "predictor.h"

/* funcao principal */
int main(int argc, char **argv) {   
    char *trainingFile;
    char *netSimFile;  
    char *testFile;
    char *predictionsOutFile;
    Config config;

    if( !getArgs(argc, argv, &trainingFile, &netSimFile, 
        &testFile, &predictionsOutFile) ) {
        return 1;
    }

    std::cout << "\nloading base...\n\n";
    Training training;
    training.loadBase(trainingFile);
    training.printBase();

    std::cout << "\ncalculating avgs...\n\n";
    training.calculateRatingsAvg();

    std::cout << "loading network...\n\n";
    Network network(config.getLoadNetSize());
    network.load(netSimFile);
    // network.print();

    std::cout << "making predictions...\n\n";
    Prediction pred;
    pred.makePredictions(testFile, &network, &training, predictionsOutFile, &config);
    
    return 0;
}

/* recebe parametros:
    - trainingFile:         arquivo com base de treino (uma linha por usuario)
    - netSimFile:           arquivo com a rede de usuarios mais similares
    - testFile:             arquivo com a base de teste
    - numNeighbs:           numero maximo de vizinhos permitidos na rede
    - predictionsOutFile:   arquivo de saida que contera as predicoes
*/
short getArgs(int argc, char **argv, char **trainingFile, char **netSimFile, 
    char **testFile, char **predictionsOutFile) {
	int opt;
    *trainingFile = NULL;
    *netSimFile = NULL;
    *testFile = NULL;
    *predictionsOutFile = NULL;

	while( (opt = getopt(argc, argv, "hb:s:t:o:")) != -1 ) {
		switch(opt) {
            case 'h':
                printUsage();
                return 0;
                break;
            case 'b':
                *trainingFile = optarg;
                break;
            case 's':
                *netSimFile = optarg;
                break;
            case 't': 
                *testFile = optarg;
                break;
            case 'o':
                *predictionsOutFile = optarg;
                break;
			default: {
                printUsage();
				return 0;
			}
		}	
	}
    if( *trainingFile == NULL    ||
        *netSimFile == NULL      ||   
        *testFile == NULL        ||
        *predictionsOutFile == NULL ) {
        printUsage();
        return 0;
    }
	return 1;
}	


void printUsage() {
    string args = "";
    args += "\t\t-b <trainingFile> ";
    args += "-s <netSimFile> ";
    args += "-t <testFile> ";
    args += "-o <predictionsOutFile> ";
    args += " [-h]\n\n";
    cout << "\n\tArgumentos corretos:\n" << args;
}


