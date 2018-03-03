#include <QtGui/QApplication>
#include <QtGui/QFont>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include "Plateau.h"
#include "lcdrange.h"

class MyWidget : public QWidget
{
 public:
 MyWidget(QWidget *parent = 0);
};

MyWidget::MyWidget(QWidget *parent): QWidget(parent)
{
/*
     QPushButton *quit = new QPushButton(tr("&Quit"));
     quit->setFont(QFont("Times", 18, QFont::Bold));

     connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
*/


 Plateau *plateau = new Plateau;

     LCDRange *score = new LCDRange;

     LCDRange *hiScore = new LCDRange;



     connect(plateau, SIGNAL(scoreChanged(int)),
             score, SLOT(setValue(int)));

     connect(plateau, SIGNAL(hiScoreChanged(int)),
             hiScore, SLOT(setValue(int)));
//score->setFont(QFont("Times", 18, QFont::Bold));

/*
     QPushButton *shoot = new QPushButton(tr("&Shoot"));
     shoot->setFont(QFont("Times", 18, QFont::Bold));
     connect(shoot, SIGNAL(clicked()), plateau, SLOT(shoot()));
*/



     QVBoxLayout *botLayout = new QVBoxLayout; //deux ensemble ¨¤ cet endroit
     botLayout->addWidget(score);
     botLayout->addWidget(hiScore);

     QGridLayout *gridLayout = new QGridLayout;
     gridLayout->addWidget(plateau, 0, 0);
     //gridLayout->addWidget(quit, 1, 0);

     gridLayout->addLayout(botLayout, 1, 0);
     gridLayout->setColumnStretch(1, 10);
     setLayout(gridLayout);



     plateau->setFocus(); //dire que lui a le focus (du clavier par exemple)!


hiScore->setValue(plateau->getHiScore());
 //QGridLayout *gridLayout = new QGridLayout;
 //gridLayout->addWidget(plateau, 0, 0, 1, 0);
 //gridLayout->setColumnStretch(1, 10);
 //setLayout(gridLayout);

}

int main(int argc, char *argv[])
{
 QApplication app(argc, argv);
 MyWidget widget;
 //widget.setGeometry(0, 0, 16*16, 16*10);
 widget.show();
 return app.exec();
}


//regrouper UpdateMove et update
//clignotement de sprite quand agrandissement du snake
//commencer dans chacune des directions possibles au d¨¦but.
//quelque chose qd on gagne, note sur 20.








