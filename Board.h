#ifndef BOARD_H
#define BOARD_H
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
#include "Alea.h"
#include "Helpers.h"
#include <string>
#include <QDebug>

class QTimer;

class Board : public QWidget
{
    Q_OBJECT
    public:
        // The board is the main window, on which the snake moves
        Board(QWidget *parent = 0);
        // Retrieve hi-score, needed to give it to LCD score's window
        int getHiScore();

    public slots:

    signals:
        // Continuous update of the score on LCD score's window
        void scoreChanged(int newScore);
        // Continuous update of the hi-score on LCD score's window
        void hiScoreChanged(int newScore);

    private slots:
        // Update the board and let the snake moves in real time
        void updateMove();

    protected:

    private:
        void begin(); //active le timer de déplacement

        void keyPressEvent(QKeyEvent *event);
        void paintEvent(QPaintEvent *event); //lance paintShot

        /*
          Board and cells
        */
        // number of cells in X
        int m_nbSquaresX;
        // number of cells in Y
        int m_nbSquaresY;
        // size of a cell in pixels ; each cells has area of m_cellSize^2
        int m_cellSize;

        /*
          Selecting areas
        */
        // Generic rectangle of size m_cell * m_cell from (x, y)
        QRect m_rect(int x, int y);

        ////
        // Area for the snake
        ////
        // Where is the head of the snake?
        QRect m_rectHeadUsual(bool affiche);
        // Where is the head while crossing a torus' corner?
        QRect m_rectHeadCorner(bool affiche);
        // m_rectHeadUsual union m_rectHeadCorner forms the complete head region
        QRegion m_rectHead();
        // Where is the i-th component of the snake's body? (from i = 1)
        QRect m_rectCorps(unsigned int i, bool affiche);
        // Where is the tail of the snake?
        QRect m_rectQueue(bool affiche);
        // For debugging, print rectangle position in console
        void afficher(bool affiche, std::string objName, QRect result) const;

        ////
        // Area for the apples
        ////
        // Where is the current apple?
        QRect m_rectPomme(bool affiche);





        QRegion m_snakeRegion();
        QRegion m_appleRegion();



        void newGame();
        void avancer();
        void testCollision(); //vérification si on touche le snake après avoir avancé. Si c'est le cas, on lance ecrire()
        void avancerUnPas(); //avance d'un pixel dans le direction en cours (modulo le plateau)
        void augmenterLongueur(); //augmente la longueur de 1
        void agrandissementSnake(); //test s'il faut augmenter maintenant le snake
        void perdre();
        void gagner();
        void nouvellePositionPomme();
        bool collisionAvecPomme();
        void pommeMangee();
        int mesureDistancePommeTete(); //distance de Manhattan (modulo les bords)




        void sauvegardePositions(); //enregistre position en cours dans m_positionsSauvegardeesX/Y
        void sauvegardePositionsModulo(); //idem modulo la taille du plateau (méthode à lancer après sauvegardePositions())
        void sauvegardeGrille();
        void sauvegardeArete();
        void sauvegardePomme();















        //timer de déplacement en temps réel
        QTimer *m_autoMoveTimer; //The QTimer class provides ''repetitive'' and single-shot timers.
        int m_vitesseTimer;



        //information sur le snake
        int m_direction; //direction en cours : 0=est ; 1=nord ; 2=ouest ; 3=sud ; -1=nonDéfini
        int m_ancienneDirection; //direction précédente (pour éviter des mouvements du snake), encore dans {-1,0,1,2,3}
        bool m_repositionnerSurLaGrille; //lié à ancienneDirection (pour éviter des mouvements du snake)
        int m_positionEnCoursX; //position dans [0, m_cellSize*m_nbSquaresX[
        int m_positionEnCoursY; //position dans [0, m_cellSize*m_nbSquaresY[
        unsigned int m_longueur; //longueur du snake hors queue (0 si seulement la tête, etc.)
        bool m_perdre;
        bool m_gagner;

        //informations sauvegardées du snake (pour les datas)
        std::vector<int> m_positionsSauvegardeesX; //dans {0, 1 ... longueurGrille-1}
        std::vector<int> m_positionsSauvegardeesY;
        std::vector<int> m_positionsSauvegardeesXmod;
        std::vector<int> m_positionsSauvegardeesYmod;
        std::vector<std::string> m_directions;

        std::vector<int> m_grilleSauvegardee; //valeurs de 0 à m_nbSquaresX*m_nbSquaresY-1
        int m_valeurMaxGrilleSauvegardee;

        std::vector<int> m_aretesEst; //valeurs de 0 à m_nbSquaresX*m_nbSquaresY-1
        std::vector<int> m_aretesNord; //valeurs de 0 à m_nbSquaresX*m_nbSquaresY-1
        std::vector<int> m_aretesOuest; //valeurs de 0 à m_nbSquaresX*m_nbSquaresY-1
        std::vector<int> m_aretesSud; //valeurs de 0 à m_nbSquaresX*m_nbSquaresY-1
        int m_valeurMaxAreteSauvegardee;

        //affichage graphique sur le plateau







        std::deque<unsigned int> m_nbPasAvantAgrandissementSnake;
        unsigned int m_nePasBougerLaQueue;

        //pommes
        int m_positionPommeX; //position dans [0, m_cellSize*m_nbSquaresX[
        int m_positionPommeY; //position dans [0, m_cellSize*m_nbSquaresY[


        std::vector<int> m_positionsSauvegardeesPommeX; //dans {0, 1 ... longueurGrille-1}
        std::vector<int> m_positionsSauvegardeesPommeY;

        std::vector<int> m_grillePomme; //valeurs de 0 à m_nbSquaresX*m_nbSquaresY-1
        int m_valeurMaxGrillePomme;

        bool pasDeCollisionAvecSerpent(int nouveauX, int nouveauY);







        int m_score;
        int m_pointsEnCours; //de 100 et décroît progressivement

        void decroitPointEnCours();
        int m_nbPasAvantDecroissancePoints;
        int m_temps;




        int m_hiScore;

        int m_partieEnCours;

        void writingHiScore();

        /////////////////////////
        // Painting and boards //
        /////////////////////////

        /*
          Painting main layer
        */
        QRect m_rectPlateau();
        void paintShot(QPainter &painter);

        /*

        */

        /*
          Painting the whole board
        */
        // Whether the board layer has changed
        bool m_changementPlateau;

        // When m_changementPlateau==1, we redraw the whole board
        void paintTout(QPainter &painter);

        /*
          Current layer and painting statistic board layers
        */
        // Current board layer
        // "" for the classic board layer
        // "densite", "arete", "nonoriente", "pomme" for statistic layers
        std::string m_quelPlateau;

        // Painting snake's density layer
        void paintDensite(QPainter &painter);
        // Painting oriented edges crossed
        void paintArete(QPainter &painter);
        QPolygon m_triangleNord(int x, int y);
        QPolygon m_triangleSud(int x, int y);
        QPolygon m_triangleEst(int x, int y);
        QPolygon m_triangleOuest(int x, int y);
        // Painting non-oriented edges crossed
        void paintNonOriente(QPainter &painter);
        // Painting apple's density layer
        void paintPomme(QPainter &painter);

        ////////////////////////
        // Whole game history //
        ////////////////////////
        /*
          Each vector has size T
          (T: number of moves/steps of the snake from beginning of this game).
          This information is only used to output a txt file containing whole
          history.
        */
        // Steps 0, 1, ..., T-1
        std::vector<int> m_tempsSave;
        // Score at each step t
        std::vector<int> m_scoreSave;
        // Number of apples eaten at each step t
        std::vector<int> m_nbPommesMangeesSave;
        // Length of snake at each step t
        std::vector<int> m_longueurSnakeSave;
        // Position X of apples at each step t (int between 0 and longueurGrille-1}
        std::vector<int> m_positionsSauvegardeesPommeXcomplet;
        // Position Y of apples at each step t
        std::vector<int> m_positionsSauvegardeesPommeYcomplet;

        /*
          Update previous vectors by adding current state
        */
        void sauvegardeTempsPointsNbPommesMangeesLongueurSnake();

        /*
          Writting previous vectors into a txt file
        */
        void writingWholeHistory();
};

#endif
