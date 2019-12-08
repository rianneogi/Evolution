#include "Rating.h"

int calculateELO(int elo1, int elo2, int res, int k_factor)
{
    double qa = pow(10.0,elo1/400.0);
    double qb = pow(10.0,elo2/400.0);
    double ea = qa/(qa+qb);
    
    return k_factor*(res-ea); //amount of ELO player1 gains and player2 loses
}