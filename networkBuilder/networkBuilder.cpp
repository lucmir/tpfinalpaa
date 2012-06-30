/************************************************************************
                        networkBuilder.cpp
************************************************************************/

#include "networkBuilder.h"

/* funcao principal */
int main(int argc, char **argv) {   
    
    char *baseFile;
    char *netOutFile;
    unsigned int minInters;
        
    /* recebe parametros */
	if( !getArgs(argc, argv, &baseFile, &netOutFile, &minInters) )
		return 1;

    /* construcao da rede */
    cout << "\n\n\nPrograma Iniciado\n";
    Network network;
 
    cout << "\n\n\nCarregando a base...\n";
    network.loadBase(baseFile);

    cout << "\n\nConstruindo rede...";
    network.buildNetwork(minInters, netOutFile);

    return 0;
}

/* recebe parametros:
 * - baseFile = arquivo com o treino (ou base completa)
 * - netOutFile = arquivo de saida que contera as relacoes da rede 
 * - minInters = numero de intersecoes minimas para haver um relacionamento
*/
short getArgs(int argc, char **argv, char **baseFile, char **netOutFile, unsigned int *minInters) {
	int opt;
    *baseFile = NULL;    
    *netOutFile = NULL;
    *minInters = 0;
	
    while( (opt = getopt(argc, argv, "hb:i:o:")) != -1 ) {
		switch(opt) {
            case 'h':
                printUsage();
                return 0;
                break;
            case 'b':
                *baseFile = optarg;
                break;
            case 'i':
                *minInters = atoi(optarg);
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
    if( *baseFile == NULL || *netOutFile == NULL || *minInters == 0 ) {
        printUsage();
        return 0;
    }
	return 1;
}	


void printUsage() {
    string args = "";
    args += "\t\t-b <baseFile> ";
    args += "-o <netOutFile> ";
    args += "-i <minInters> ";
    args += " [-h]\n\n";
    cout << "\n\tArgumentos corretos:\n" << args;
}
