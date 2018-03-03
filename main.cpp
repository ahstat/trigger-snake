#include <QtGui/QApplication>
#include <QtGui/QFont>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include "Board.h"
#include "LCDRange.h"

class MyWidget : public QWidget
{
    public:
        MyWidget(QWidget *parent = 0);
};

MyWidget::MyWidget(QWidget *parent): QWidget(parent)
{
    Board *board = new Board;
    LCDRange *score = new LCDRange;
    LCDRange *hiScore = new LCDRange;

    connect(board, SIGNAL(scoreChanged(int)),
            score, SLOT(setValue(int)));

    connect(board, SIGNAL(hiScoreChanged(int)),
            hiScore, SLOT(setValue(int)));

    QVBoxLayout *boxLayout = new QVBoxLayout;

    // Two elements together at that place
    boxLayout -> addWidget(score);
    boxLayout -> addWidget(hiScore);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout -> addWidget(board, 0, 0);
    gridLayout -> addLayout(boxLayout, 1, 0);
    gridLayout -> setColumnStretch(1, 10);
    setLayout(gridLayout);

    // To say that it should keep focus (of the keyboard for example)
    board -> setFocus();
    hiScore -> setValue(board -> getHiScore());
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyWidget widget;
    widget.show();
    return app.exec();
}
