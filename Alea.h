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


// La classe pour la generation de nombres pseudo-aleatoires interfacee avec MTRand
class Alea
{

 public:
 static double unif();
 static double exp(double lambda); //Renvoie une v.a. exponentielle de paramètre lambda
 static bool coin(double p); // Renvoie une v.a. de Bernoulli de parametre p
 static long entier_unif_0_n_1(long n); // Renvoie une v.a. entiere uniforme entre 0 et n-1
 //static long poisson(const double & lambda); // Renvoie une v.a. de loi de Poisson de paramètre lambda
 //static int tirageParmi(std::vector<double> & comptage); //préférer walker si on utilise en boucle cette méthode.

 //static double moyenne(std::vector<double> termes);
 //static double ecarttype(std::vector<double> termes);
};

//

#endif
