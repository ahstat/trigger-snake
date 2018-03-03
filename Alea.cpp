#include "Alea.h"
using namespace std;
MTRand gRand(time(0)); // seed is different at each execution

// Sample an element from uniform distribution in (0, 1)
double Alea::unif()
{
    double u(gRand.rand());
    if(u!=0 && u!=1)
    {
        return gRand.rand(); //gRand can be found in MTRand()
    }
    else
    {
        return unif();
    }
}

// Integer following uniform distribution in {0, 1, ..., n-1}
long Alea::rand_integer(long n)
{
    return((long) floor(unif() * ((double) n) ));
}
