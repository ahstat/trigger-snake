#include "Helpers.h"
using namespace std;

int Helpers::base(int nombre, int base)
{
    //std::cout << nombre << std::endl;
    //std::cout << "Debut" << std::endl;
    int nombreFinal(0);

    int n(1);
    int nombreDiminuant(nombre);
    while(nombreDiminuant != 0)
    {
        //std::cout << nombreDiminuant % base << std::endl;
        nombreFinal+= (nombreDiminuant % base)*n;
        n*=10;
        nombreDiminuant/=base;
    }
    //std::cout << nombreFinal << std::endl;
    return nombreFinal;
}

int Helpers::lireHiScore()
{
    QFile data("hiscore.txt");
    if(data.exists()==0)
    {
        //std::cout << "le fichier n'existe pas " << std::endl;
        return 0;
    }

    if(!data.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::cout << "ERREUR lecture " << std::endl;
        return 0;
    }

    QString lines;
    //data.open(QIODevice::ReadOnly);
    int index(0);
    while(!data.atEnd())
    {
        lines = data.readLine();
        index++;
    }
    //std::cout << lines.toStdString() << std::endl;

    bool ok;
    //int hex = str.toInt(&ok, 16);       // hex == 255, ok == true
    //int dec = str.toInt(&ok, 10);       // dec == 0, ok == false

    int nombre9(lines.split("	")[0].toInt(&ok, 9));//convert the first part to Int
    int nombre7(lines.split("	")[1].toInt(&ok, 7));//convert the first part to Int
    int nombre8(lines.split("	")[2].toInt(&ok, 8));//convert the first part to Int
    //std::cout << "Le hi-score est : " << nombre9 << " " << nombre7 << " " << nombre8 << std::endl;

    if(nombre9 != nombre8 || nombre8 != nombre7 || nombre9 > 20000)
    {
        //std::cout << "ERREUR " << nombre9 <<"!=" <<nombre8 << " OU " <<  nombre8 <<"!=" << nombre7 << " OU " <<  nombre9 <<"> 20000" << std::endl;
        return 0;
    }
    else
    {
        //std::cout << "OK " << nombre9 <<"!=" <<nombre8 << " OU " <<  nombre8 <<"!=" << nombre7 << " OU " <<  nombre9 <<"> 20000" << std::endl;
        return nombre9;
    }
}

std::string Helpers::convertDirection(int direction)
{
    if(direction==0)
    {
        return "est";
    }
    else if(direction==1)
    {
        return "nord";
    }
    else if(direction==2)
    {
        return "ouest";
    }
    else if(direction==3)
    {
        return "sud";
    }
    else
    {
        std::cout << "ERREUR" << std::endl;
        return "NA";
    }
}

int Helpers::lirePartieEnCours()
{
    QFile data("output.txt");
    if(data.exists()==0)
    {
        //std::cout << "le fichier n'existe pas " << std::endl;
        return 0;
    }

    if(!data.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::cout << "ERREUR lecture " << std::endl;
        return 0;
    }

    QString lines;
    //data.open(QIODevice::ReadOnly);
    int index(0);
    while(!data.atEnd())
    {
        lines = data.readLine();
        index++;
    }
    //std::cout << lines.toStdString() << std::endl;
    int nombre(lines.split("	")[0].toInt());//convert the first part to Int
    //std::cout << nombre << std::endl;
    return nombre+1;
}
