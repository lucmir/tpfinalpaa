/************************************************************************
                        networkBuilder.h
************************************************************************/

#ifndef NETWORKBUILDER_H
#define NETWORKBUILDER_H

#include <vector>
#include <istream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <list>
#include <map>
#include <math.h>
#include <pthread.h>

#include "network.h"

#define MIN_INTENS      1

using namespace std;

int main(int argc, char **argv);
short getArgs(int argc, char **argv, char **baseFile, char **testUserListFile, char **usersRatingsAvgFile, char **netOutFile, unsigned int *minInters, unsigned int *maxNeighbs, unsigned int *numThreads);
void printUsage();


#endif
