#ifndef HELPERS_H
#define HELPERS_H
#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <math.h>
#include <iostream>
#include <queue>
#include <algorithm>

class Helpers
{
    public:
        static int base(int nombre, int base);
        static int lireHiScore();
        static std::string convertirDirection(int direction); //0=est ; 1=nord ; 2=ouest ; 3=sud ; -1=nonDéfini
        static int lirePartieEnCours();
};

#endif
