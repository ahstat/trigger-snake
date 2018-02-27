#include "Alea.h"
using namespace std;
MTRand gRand(time(0)); //si on veut une graine qui change à chaque exécution
//MTRand gRand(25); //graine fixe (le générateur peut prendre les valeurs zéro et une)


double Alea::unif()
{
 double u(gRand.rand());
 if( u!=0 && u!=1 )
 {
  return gRand.rand();//gRand dans MTRand();
 }
 else
 {
  return unif();
 }
}

double Alea::exp(double lambda)
{
 return (-1/lambda)*log(unif()); //log est bien le log népérien.
}

bool Alea::coin(double p)
{
 if(unif()<p) 
 {
  return(true);
 } 
 else 
 {
  return(false);
 }
}

long Alea::entier_unif_0_n_1(long n)
{
 return((long) floor(unif()* ((double) n) ));
}

/*
long Alea::poisson(const double & lambda) // La méthode par rejet et comparaison avec une loi de Cauchy pour les grandes valeurs, reprise des Numerical Recipes in C++
{
 const double pi=3.141592653589793238;
 static double interm_raccarree, interm_log, interm_loggamma, interm_exp, lambda_prec;
 long valeur;
 double interm_mult, cauchy, valeur_reelle, seuil_unif;
 if(lambda<12.0)
 {
  if(lambda != lambda_prec)
  {
   lambda_prec=lambda ; interm_exp = exp(-lambda);}
   valeur=-1;
   interm_mult=1.0;
   do
   {
    valeur++;
    interm_mult *= Alea::unif();
   }
   while(interm_mult>interm_exp);
  }
 else
 {
  if(lambda != lambda_prec)
  {
   lambda_prec = lambda; interm_raccarree = sqrt(2.0*lambda);
   interm_log = log(lambda) ; interm_loggamma = lambda*interm_log - loggamma(lambda+1.0); 
  }
  do
  {
   do
   {
    cauchy=tan(pi*unif());
    valeur_reelle=interm_raccarree*cauchy+lambda;
   } while(valeur_reelle<0.0);
   valeur=long(floor(valeur_reelle));
   seuil_unif=0.9*(1.0+cauchy*cauchy)*exp(valeur*interm_log-loggamma(valeur+1.0)-interm_loggamma);
  } while(unif()>seuil_unif);
 }
 return(valeur);
}
*/

/*
int Alea::tirageParmi( std::vector<double> & comptage )
{
 double parametre(0);
 for( int i(0); i < comptage.size(); i++) //initialisation. 
 {
  parametre += comptage[i];
  //cout << comptage[i] << " est comptage[" << i << "]." << endl;
 }
 double u(Alea::unif()*parametre); //nombre uniforme dans [0,parametre].
 double sommePartielle(0); //somme partielle du vecteur comptage.
 int sortie(-1);
 for (int j(0); j < comptage.size(); j++) 
 {
  if(sommePartielle <= u  &&  u < sommePartielle + comptage[j] )
  { 
   sortie = j;
  } 
  sommePartielle = sommePartielle + comptage[j];
  // cout << "On est à l'étape " << j << " et la somme partielle est " << sommePartielle << endl;
 }

 if( sommePartielle == u && sortie == -1) //si le nombre Aleatoire tiré est exactement 1 (parmi [0,1]), alors sommePartielle == u et on prend le dernier.
 {
  //cout << "On passe ici, très peu fréquent (Alea)" << endl; //debug
  for (int j(0); j < comptage.size(); j++) 
  {
   if(comptage[j] != 0)
   {
    sortie = j;
   }
  }
 }

 if( sortie == -1)
 {
  cout << "ERREUR dans Alea tirageParmi, la sortie est -1. On a : comptage.size() =" << comptage.size() << "// parametre = " << parametre << "//" << endl;
  for (int j(0); j < comptage.size(); j++) 
  {
   cout << "comptage[" << j << "]=" << comptage[j] << "//";
  }
  cout << "Le nombre uniforme dans [0,parametre] est : " << u << endl;
  cout << "La valeur de sommeParticielle apres tous les ajouts est : " << sommePartielle << endl;
  cout << endl;

  cout << "Pour éviter l'erreur, on fixe arbitrairement par la derniere valeur non nulle :" << endl << endl;
  for (int j(0); j < comptage.size(); j++) 
  {
   if(comptage[j] != 0)
   {
    sortie = j;
   }
  }
 }

 return sortie;
}


double Alea::moyenne(vector<double> termes)
{
 double sortie(0);
 for (int i(0); i < termes.size(); i++) 
 {
  sortie+=termes[i];
 }
 return sortie/termes.size();
}

double Alea::ecarttype(vector<double> termes)
{
 double sortie2(0);
 for (int i(0); i < termes.size(); i++) 
 {
  sortie2+=termes[i]*termes[i];
 }
 return sqrt(sortie2/termes.size()-moyenne(termes)*moyenne(termes));
}



*/


