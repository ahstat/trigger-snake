#include "Helpers.h"
using namespace std;

int Helpers::base(int number, int base)
{
    int finalNumber(0);

    int n(1);
    int decreasingNumber(number);
    while(decreasingNumber != 0)
    {
        finalNumber+= (decreasingNumber % base)*n;
        n*=10;
        decreasingNumber/=base;
    }
    return finalNumber;
}

int Helpers::readHiScore()
{
    ////
    // Read hiscore coded in bases 9, 7, 8.
    ////
    QFile data("hiscore.txt");
    if(data.exists()==0)
    {
        return 0;
    }

    if(!data.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::cout << "ERROR reading" << std::endl;
        return 0;
    }

    QString lines;
    int index(0);
    while(!data.atEnd())
    {
        lines = data.readLine();
        index++;
    }

    ////
    // Convert read numbers into base 10
    ////
    bool ok;
    int number9(lines.split("	")[0].toInt(&ok, 9));
    int number7(lines.split("	")[1].toInt(&ok, 7));
    int number8(lines.split("	")[2].toInt(&ok, 8));

    ////
    // Check that those numbers are equals and smaller than 20000
    ////
    if(number9 != number8 || number8 != number7 || number9 > 20000)
    {
        return 0;
    }
    else
    {
        return number9;
    }
}

std::string Helpers::convertDirection(int direction)
{
    if(direction==0)
    {
        return "east";
    }
    else if(direction==1)
    {
        return "north";
    }
    else if(direction==2)
    {
        return "west";
    }
    else if(direction==3)
    {
        return "south";
    }
    else
    {
        std::cout << "ERROR" << std::endl;
        return "NA";
    }
}

int Helpers::getCurrentPlayId()
{
    QFile data("output.txt");

    // Game 0 if file doesn't exist
    if(data.exists()==0)
    {
        return 0;
    }

    // Game 0 if cannot read the file
    if(!data.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::cout << "ERROR reading" << std::endl;
        return 0;
    }

    // Extract most recent game number and add 1
    QString lines;
    int index(0);
    while(!data.atEnd())
    {
        lines = data.readLine();
        index++;
    }
    int number(lines.split("	")[0].toInt());
    return number+1;
}
