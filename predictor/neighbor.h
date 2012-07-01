/************************************************************************
                            neighbor.h
************************************************************************/

#ifndef NEIGHBOR_H
#define NEIGHBOR_H

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


/* estrutura que define um vizinho na rede */
struct Neighbor {
    unsigned long int userId;
    unsigned int inters;
    float cosinoIntens;
    float pearsonIntens;
    float represent;

    void init(unsigned long int u, unsigned int n, float ic, float ip, float r);
    void print();
};


/* Ic x R */
bool neighbCmp1(Neighbor a, Neighbor b);
/* Ip x R */
bool neighbCmp2(Neighbor a, Neighbor b);
/* N x Ic x R */
bool neighbCmp3(Neighbor a, Neighbor b);
/* n x ip x r */
bool neighbCmp4(Neighbor a, Neighbor b);


#endif
