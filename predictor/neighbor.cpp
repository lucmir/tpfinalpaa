/************************************************************************
                            neighbor.cpp
************************************************************************/

#include "neighbor.h"


void Neighbor::print() {
    std::cout << userId << " " << inters << " " << cosinoIntens << " " << pearsonIntens << " " << represent << "\n"; 
}

void Neighbor::init(unsigned long int u, unsigned int n, float ic, float ip, float r) {
    userId = u;
    inters = n;
    cosinoIntens = ic;
    pearsonIntens = ip;
    represent = r;
}

/* funcoes de comparacao utilizadas na ordenacao dos vizinhos */

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

