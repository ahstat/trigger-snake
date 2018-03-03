#ifndef DEF_ALEA
#define DEF_ALEA

#if defined __GNUC__
#pragma GCC system_header
#elif defined __SUNPRO_CC
#pragma disable_warn
#elif defined _MSC_VER
#pragma warning(push, 1)
#endif

#include "MersenneTwister.h"

#if defined __SUNPRO_CC
#pragma enable_warn
#elif defined _MSC_VER
#pragma warning(pop)
#endif
#include <vector>

// Class to sample from some distributions using MTRand pseudo-random generator
class Alea
{
    public:
        // Sample an element from uniform distribution in (0, 1)
        static double unif();
        // Integer following uniform distribution in {0, 1, ..., n-1}
        static long rand_integer(long n);
};

#endif
