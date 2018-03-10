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
        // Convert a number of a base into base 10
        static int base(int number, int base);
        // Read hi-score from a "hiscore.txt" file
        static int readHiScore();
        // Convert 0, 1, 2, 3 into "east", "north", "west", "south" (resp.)
        static std::string convertDirection(int direction);
        // Extract which is current play from "output.txt".
        // it is game 0 if file doesn't exist
        static int getCurrentPlayId();
};

#endif
