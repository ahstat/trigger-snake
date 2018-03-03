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
#include <string>
#include <QDebug>

class QTimer;

class Board : public QWidget
{
    Q_OBJECT
    public:
        Board(QWidget *parent = 0); //constructeur (pas de fenêtre parent)
        void avancer();
        void sauvegardePositions(); //enregistre position en cours dans m_positionsSauvegardeesX/Y
        void sauvegardePositionsModulo(); //idem modulo la taille du plateau (méthode à lancer après sauvegardePositions())
        void sauvegardeGrille();
        void sauvegardeArete();
        void sauvegardePomme();
        void sauvegardeTempsPointsNbPommesMangeesLongueurSnake();

        void testCollision(); //vérification si on touche le snake après avoir avancé. Si c'est le cas, on lance ecrire()
        void avancerUnPas(); //avance d'un pixel dans le direction en cours (modulo le plateau)
        std::string convertirDirection(int direction); //0=est ; 1=nord ; 2=ouest ; 3=sud ; -1=nonDéfini

        void augmenterLongueur(); //augmente la longueur de 1
        void agrandissementSnake(); //test s'il faut augmenter maintenant le snake

        void perdre();
        void gagner();
        void ecrire();
        int lirePartieEnCours();

        void nouvellePositionPomme();
        bool collisionAvecPomme();
        void pommeMangee();

        int mesureDistancePommeTete(); //distance de Manhattan (modulo les bords)

        void newGame();
        int getHiScore();
        void ecrireNom(char k);

    public slots:
        void shoot(); //active le timer de déplacement

    signals:
        void scoreChanged(int newScore);
        void hiScoreChanged(int newScore);

    private slots:
        void updateMove(); //update le plateau et lance l'avancement d'un pas du snake
        void updateSeulement(); //update tout

    protected:
        void paintEvent(QPaintEvent *event); //lance paintShot
        void keyPressEvent(QKeyEvent *event);

    private:
        //timer de déplacement en temps réel
        QTimer *m_autoMoveTimer; //The QTimer class provides ''repetitive'' and single-shot timers.
        int m_vitesseTimer;

        //informations graphiques
        int m_grilleLongueur; //nombre de cases de la grille en longueur
        int m_grilleHauteur; //idem en hauteur
        int m_tailleTete; //une case mesure m_tailleTete^2

        //information sur le snake
        int m_direction; //direction en cours : 0=est ; 1=nord ; 2=ouest ; 3=sud ; -1=nonDéfini
        int m_ancienneDirection; //direction précédente (pour éviter des mouvements du snake), encore dans {-1,0,1,2,3}
        bool m_repositionnerSurLaGrille; //lié à ancienneDirection (pour éviter des mouvements du snake)
        int m_positionEnCoursX; //position dans [0, m_tailleTete*m_grilleLongueur[
        int m_positionEnCoursY; //position dans [0, m_tailleTete*m_grilleHauteur[
        unsigned int m_longueur; //longueur du snake hors queue (0 si seulement la tête, etc.)
        bool m_perdre;
        bool m_gagner;

        //informations sauvegardées du snake (pour les datas)
        std::vector<int> m_positionsSauvegardeesX; //dans {0, 1 ... longueurGrille-1}
        std::vector<int> m_positionsSauvegardeesY;
        std::vector<int> m_positionsSauvegardeesXmod;
        std::vector<int> m_positionsSauvegardeesYmod;
        std::vector<std::string> m_directions;

        std::vector<int> m_grilleSauvegardee; //valeurs de 0 à m_grilleLongueur*m_grilleHauteur-1
        int m_valeurMaxGrilleSauvegardee;

        std::vector<int> m_aretesEst; //valeurs de 0 à m_grilleLongueur*m_grilleHauteur-1
        std::vector<int> m_aretesNord; //valeurs de 0 à m_grilleLongueur*m_grilleHauteur-1
        std::vector<int> m_aretesOuest; //valeurs de 0 à m_grilleLongueur*m_grilleHauteur-1
        std::vector<int> m_aretesSud; //valeurs de 0 à m_grilleLongueur*m_grilleHauteur-1
        int m_valeurMaxAreteSauvegardee;

        //affichage graphique sur le plateau
        void paintShot(QPainter &painter);
        void paintDensite(QPainter &painter);
        void paintArete(QPainter &painter);
        void paintNonOriente(QPainter &painter);
        void paintPomme(QPainter &painter);
        void paintTout(QPainter &painter);
        QRect m_rectTete(bool affiche); //tête non fixée sur la grille (sauf quand on perd)
        QRect m_rectTeteBord(bool affiche); //quand la tête traverse le tore
        QRect m_rectCorps(unsigned int i, bool affiche); //corps du snake fixé sur la grille. associé à m_positionsSauvegardeesX/Y, le rectangle correspondant (indépendant des positions X/Y en cours). i entier à partir de 1.
        QRect m_rectQueue(bool affiche); //queue non fixée sur la grille (sauf quand on perd)
        void afficher(QRect result) const; //pour debug, affiche la position du rectangle

        QRect m_rectEst(int x, int y);
        QRect m_rectNord(int x, int y);
        QRect m_rectOuest(int x, int y);
        QRect m_rectSud(int x, int y);

        QRect m_rect(int x, int y);

        std::deque<unsigned int> m_nbPasAvantAgrandissementSnake;
        unsigned int m_nePasBougerLaQueue;

        //pommes
        int m_positionPommeX; //position dans [0, m_tailleTete*m_grilleLongueur[
        int m_positionPommeY; //position dans [0, m_tailleTete*m_grilleHauteur[
        QRect m_rectPomme(bool affiche); //position dans [0, m_tailleTete*m_grilleHauteur[
        std::vector<int> m_positionsSauvegardeesPommeX; //dans {0, 1 ... longueurGrille-1}
        std::vector<int> m_positionsSauvegardeesPommeY;

        std::vector<int> m_grillePomme; //valeurs de 0 à m_grilleLongueur*m_grilleHauteur-1
        int m_valeurMaxGrillePomme;

        bool pasDeCollisionAvecSerpent(int nouveauX, int nouveauY);
        std::string m_quelPlateau;

        QRect m_rectPlateau();
        bool m_changementPlateau;

        QPolygon m_triangleNord(int x, int y);
        QPolygon m_triangleSud(int x, int y);
        QPolygon m_triangleEst(int x, int y);
        QPolygon m_triangleOuest(int x, int y);

        int base(int nombre, int base);

        int lireHiScore();
        int m_score;
        int m_pointsEnCours; //de 100 et décroît progressivement

        void decroitPointEnCours();
        int m_nbPasAvantDecroissancePoints;
        int m_temps;

        std::vector<int> m_tempsSave;
        std::vector<int> m_scoreSave;
        std::vector<int> m_nbPommesMangeesSave;
        std::vector<int> m_longueurSnakeSave;
        std::vector<int> m_positionsSauvegardeesPommeXcomplet; //dans {0, 1 ... longueurGrille-1}
        std::vector<int> m_positionsSauvegardeesPommeYcomplet;

        int m_partieEnCours;

        int m_hiScore;

        int m_A;
        int m_E;
        int m_F;
        int m_I;
        int m_M;
        int m_O;
        int m_Y;
        int m_Z;
};

#endif
