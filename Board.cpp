#include "Board.h"

Board::Board(QWidget *parent) : QWidget(parent)
{
    //timer de déplacement en temps réel
    m_autoMoveTimer = new QTimer(this); //timer qui démarre dès que shoot() est appelé.
    connect(m_autoMoveTimer, SIGNAL(timeout()), this, SLOT(updateMove())); //We'll move the shot every time the timer times out
    m_vitesseTimer=3; //timer en millisecondes -- mettre 3 pour la vraie vitesse

    //informations graphiques
    m_grilleLongueur=16;
    m_grilleHauteur=8;
    m_tailleTete=48; //mieux si congru modulo 4
    setPalette(QPalette(QColor(250, 250, 200)));
    setAutoFillBackground(true);
    setFixedSize(m_tailleTete*m_grilleLongueur, m_tailleTete*m_grilleHauteur);

    //information sur le snake
    m_direction=0;
    m_ancienneDirection=-1;

    //m_positionEnCoursX=0;
    //m_positionEnCoursY=0; //en haut à gauche au départ
    m_positionEnCoursX=m_tailleTete*Alea::rand_integer(m_grilleLongueur);
    m_positionEnCoursY=m_tailleTete*Alea::rand_integer(m_grilleHauteur); //position init aléatoire

    m_longueur = 1;
    m_perdre=FALSE;
    m_gagner=FALSE;
    m_repositionnerSurLaGrille=FALSE;

    //informations sauvegardées du snake (pour les datas)
    m_positionsSauvegardeesX.resize(0);
    m_positionsSauvegardeesY.resize(0);
    m_positionsSauvegardeesX.push_back(m_positionEnCoursX/m_tailleTete);
    m_positionsSauvegardeesY.push_back(m_positionEnCoursY/m_tailleTete); //cf initialement, position en (0,0) sur la grille
    m_positionsSauvegardeesXmod.resize(0);
    m_positionsSauvegardeesYmod.resize(0);
    m_positionsSauvegardeesXmod.push_back(m_positionEnCoursX/m_tailleTete);
    m_positionsSauvegardeesYmod.push_back(m_positionEnCoursY/m_tailleTete); //cf initialement, position en (0,0) sur la grille
    m_directions.resize(0);
    m_grilleSauvegardee.resize(m_grilleLongueur*m_grilleHauteur);
    m_grillePomme.resize(m_grilleLongueur*m_grilleHauteur);
    m_aretesEst.resize(m_grilleLongueur*m_grilleHauteur);
    m_aretesNord.resize(m_grilleLongueur*m_grilleHauteur);
    m_aretesOuest.resize(m_grilleLongueur*m_grilleHauteur);
    m_aretesSud.resize(m_grilleLongueur*m_grilleHauteur);
    for(unsigned int i(0); i<m_grilleSauvegardee.size(); i++)
    {
        m_grilleSauvegardee[i]=0;
        m_grillePomme[i]=0;
        m_aretesEst[i]=0;
        m_aretesNord[i]=0;
        m_aretesOuest[i]=0;
        m_aretesSud[i]=0;
    }
    sauvegardeGrille();
    //AFFICHAGE
    //std::cout << "init";
    //std::cout << "	(" << m_positionsSauvegardeesX.back() << "," << m_positionsSauvegardeesY.back() << ")";
    //std::cout << "	(" << m_positionsSauvegardeesXmod.back() << "," << m_positionsSauvegardeesYmod.back() << ")" << std::endl;
    m_nbPasAvantAgrandissementSnake.resize(0);
    m_nePasBougerLaQueue = 0;

    m_positionsSauvegardeesPommeX.resize(0);
    m_positionsSauvegardeesPommeY.resize(0);
    nouvellePositionPomme();

    m_quelPlateau=""; //m_voirDensite=0;
    m_valeurMaxGrilleSauvegardee = 0;
    m_valeurMaxAreteSauvegardee = 0;

    m_changementPlateau=0;

    m_valeurMaxGrillePomme=0;

    m_score=0;
    scoreChanged(m_score);
    m_pointsEnCours=100;

    m_nbPasAvantDecroissancePoints=mesureDistancePommeTete();

    m_temps=0;

    m_tempsSave.resize(0);
    m_scoreSave.resize(0);
    m_nbPommesMangeesSave.resize(0);
    m_longueurSnakeSave.resize(0);
    m_tempsSave.push_back(0);
    m_scoreSave.push_back(0);
    m_nbPommesMangeesSave.push_back(0);
    m_longueurSnakeSave.push_back(m_longueur+1);

    m_positionsSauvegardeesPommeXcomplet.resize(0);
    m_positionsSauvegardeesPommeYcomplet.resize(0);
    m_positionsSauvegardeesPommeXcomplet.push_back(m_positionsSauvegardeesPommeX.back());
    m_positionsSauvegardeesPommeYcomplet.push_back(m_positionsSauvegardeesPommeY.back());

    m_partieEnCours = lirePartieEnCours();

    m_hiScore=lireHiScore();
    //std::cout << m_hiScore << " est le hiScore" << std::endl;
    hiScoreChanged(m_hiScore);

    m_A=0;
    m_E=0;
    m_F=0;
    m_I=0;
    m_M=0;
    m_O=0;
    m_Y=0;
    m_Z=0;
}

void Board::newGame()
{
    //m_autoMoveTimer = new QTimer(this); //timer qui démarre dès que shoot() est appelé.
    //connect(m_autoMoveTimer, SIGNAL(timeout()), this, SLOT(updateMove())); //We'll move the shot every time the timer times out
    m_vitesseTimer=3; //timer en millisecondes -- mettre 3 pour la vraie vitesse

    //informations graphiques
    m_grilleLongueur=16;
    m_grilleHauteur=8;
    m_tailleTete=48; //mieux si congru modulo 4
    setPalette(QPalette(QColor(250, 250, 200)));
    setAutoFillBackground(true);
    setFixedSize(m_tailleTete*m_grilleLongueur, m_tailleTete*m_grilleHauteur);

    //information sur le snake
    m_direction=0;
    m_ancienneDirection=-1;

    m_positionEnCoursX=m_tailleTete*Alea::rand_integer(m_grilleLongueur);
    m_positionEnCoursY=m_tailleTete*Alea::rand_integer(m_grilleHauteur); //position init aléatoire

    m_longueur = 1;
    m_perdre=FALSE;
    m_gagner=FALSE;
    m_repositionnerSurLaGrille=FALSE;

    //informations sauvegardées du snake (pour les datas)
    m_positionsSauvegardeesX.resize(0);
    m_positionsSauvegardeesY.resize(0);
    m_positionsSauvegardeesX.push_back(m_positionEnCoursX/m_tailleTete);
    m_positionsSauvegardeesY.push_back(m_positionEnCoursY/m_tailleTete); //cf initialement, position en (0,0) sur la grille
    m_positionsSauvegardeesXmod.resize(0);
    m_positionsSauvegardeesYmod.resize(0);
    m_positionsSauvegardeesXmod.push_back(m_positionEnCoursX/m_tailleTete);
    m_positionsSauvegardeesYmod.push_back(m_positionEnCoursY/m_tailleTete); //cf initialement, position en (0,0) sur la grille
    m_directions.resize(0);
    m_grilleSauvegardee.resize(m_grilleLongueur*m_grilleHauteur);
    m_grillePomme.resize(m_grilleLongueur*m_grilleHauteur);
    m_aretesEst.resize(m_grilleLongueur*m_grilleHauteur);
    m_aretesNord.resize(m_grilleLongueur*m_grilleHauteur);
    m_aretesOuest.resize(m_grilleLongueur*m_grilleHauteur);
    m_aretesSud.resize(m_grilleLongueur*m_grilleHauteur);

    for(unsigned int i(0); i<m_grilleSauvegardee.size(); i++)
    {
        m_grilleSauvegardee[i]=0;
        m_grillePomme[i]=0;
        m_aretesEst[i]=0;
        m_aretesNord[i]=0;
        m_aretesOuest[i]=0;
        m_aretesSud[i]=0;
    }

    sauvegardeGrille();
    //AFFICHAGE
    //std::cout << "init";
    //std::cout << "	(" << m_positionsSauvegardeesX.back() << "," << m_positionsSauvegardeesY.back() << ")";
    //std::cout << "	(" << m_positionsSauvegardeesXmod.back() << "," << m_positionsSauvegardeesYmod.back() << ")" << std::endl;

    m_nbPasAvantAgrandissementSnake.resize(0);
    m_nePasBougerLaQueue = 0;

    m_positionsSauvegardeesPommeX.resize(0);
    m_positionsSauvegardeesPommeY.resize(0);
    nouvellePositionPomme();

    m_quelPlateau=""; //m_voirDensite=0;
    m_valeurMaxGrilleSauvegardee = 0;
    m_valeurMaxAreteSauvegardee = 0;

    m_changementPlateau=0;

    m_valeurMaxGrillePomme=0;

    m_score=0;
    scoreChanged(m_score);
    m_pointsEnCours=100;

    m_nbPasAvantDecroissancePoints=mesureDistancePommeTete();

    m_temps=0;

    m_tempsSave.resize(0);
    m_scoreSave.resize(0);
    m_nbPommesMangeesSave.resize(0);
    m_longueurSnakeSave.resize(0);
    m_tempsSave.push_back(0);
    m_scoreSave.push_back(0);
    m_nbPommesMangeesSave.push_back(0);
    m_longueurSnakeSave.push_back(m_longueur+1);

    m_positionsSauvegardeesPommeXcomplet.resize(0);
    m_positionsSauvegardeesPommeYcomplet.resize(0);
    m_positionsSauvegardeesPommeXcomplet.push_back(m_positionsSauvegardeesPommeX.back());
    m_positionsSauvegardeesPommeYcomplet.push_back(m_positionsSauvegardeesPommeY.back());

    m_partieEnCours = lirePartieEnCours();

    m_hiScore=lireHiScore();
    hiScoreChanged(m_hiScore);

    m_A=0;
    m_E=0;
    m_F=0;
    m_I=0;
    m_M=0;
    m_O=0;
    m_Y=0;
    m_Z=0;

    //ce qui change par rapport au constructeur
    m_changementPlateau=1;
    m_autoMoveTimer->stop();
    updateMove();
}

void Board::keyPressEvent(QKeyEvent *event)
{
    if(m_perdre==TRUE)
    {
        switch (event->key())
        {
            case Qt::Key_A:
                m_A++;
                if(m_A==3)
                {
                    ecrireNom('A');
                    m_A=0;
                }
                break;

            case Qt::Key_E:
                m_E++;
                if(m_E==3)
                {
                    ecrireNom('E');
                    m_E=0;
                }
                break;

            case Qt::Key_F:
                m_F++;
                if(m_F==3)
                {
                    ecrireNom('F');
                    m_F=0;
                }
                break;

            case Qt::Key_I:
                m_I++;
                if(m_I==3)
                {
                    ecrireNom('I');
                    m_I=0;
                }
                break;

            case Qt::Key_M:
                m_M++;
                if(m_M==3)
                {
                    ecrireNom('M');
                    m_M=0;
                }
                break;

            case Qt::Key_O:
                m_O++;
                if(m_O==3)
                {
                    ecrireNom('O');
                    m_O=0;
                }
                break;

            case Qt::Key_Y:
                m_Y++;
                if(m_Y==3)
                {
                    ecrireNom('Y');
                    m_Y=0;
                }
                break;

            case Qt::Key_Z:
                m_Z++;
                if(m_Y==3)
                {
                    ecrireNom('Z');
                    m_Z=0;
                }
                break;
        }
    }

    if(m_perdre==FALSE)
    {
        switch (event->key())
        {
            case Qt::Key_Up:
                if(m_ancienneDirection != 1 && m_ancienneDirection != 3)
                {
                    m_direction=1;
                    m_repositionnerSurLaGrille=TRUE;
                }
                break;

            case Qt::Key_Down:
                if(m_ancienneDirection != 3 && m_ancienneDirection != 1)
                {
                    m_direction=3;
                    m_repositionnerSurLaGrille=TRUE;
                }
                break;

            case Qt::Key_Left:
                if(m_ancienneDirection != 2 && m_ancienneDirection != 0)
                {
                    m_direction=2;
                    m_repositionnerSurLaGrille=TRUE;
                }
                break;

            case Qt::Key_Right:
                if(m_ancienneDirection != 0 && m_ancienneDirection != 2)
                {
                    m_direction=0;
                    m_repositionnerSurLaGrille=TRUE;
                }
                break;

            case Qt::Key_2:
                augmenterLongueur();
                m_score=0;
                break;

            case Qt::Key_1:
                nouvellePositionPomme();
                m_score=0;
                break;

            case Qt::Key_3:
                m_score=0;
                m_temps=1000;
                m_positionsSauvegardeesPommeX.resize(101);
                gagner();
                break;
        }
    }

    if (!m_autoMoveTimer->isActive() && m_perdre==FALSE)
    {
        shoot();
        //updateMove();
    }

    switch (event->key())
    {
        case Qt::Key_D:
            m_changementPlateau=1;
            if(m_quelPlateau != "densite")
            {
                m_quelPlateau="densite";
            }
            else
            {
                m_quelPlateau="";
            }
            //AFFICHAGE
            //std::cout << "D" << std::endl;
            break;

        case Qt::Key_Q:
            m_changementPlateau=1;
            if(m_quelPlateau != "arete")
            {
                m_quelPlateau="arete";
            }
            else
            {
                m_quelPlateau="";
            }
            //AFFICHAGE
            //std::cout << "F" << std::endl;
            break;

        case Qt::Key_S:
            m_changementPlateau=1;
            if(m_quelPlateau != "nonoriente")
            {
                m_quelPlateau="nonoriente";
            }
            else
            {
                m_quelPlateau="";
            }
            //AFFICHAGE
            //std::cout << "G" << std::endl;
            break;

        case Qt::Key_P:
            m_changementPlateau=1;
            if(m_quelPlateau != "pomme")
            {
                m_quelPlateau="pomme";
            }
            else
            {
                m_quelPlateau="";
            }
            //AFFICHAGE
            //std::cout << "P" << std::endl;
            break;


        case Qt::Key_R:
            //AFFICHAGE
            //std::cout << "R" << std::endl;
            newGame();
            break;
    }
}

int Board::getHiScore()
{
    return m_hiScore;
}

void Board::shoot()
{
    if(m_autoMoveTimer->isActive())
    {
        return;
    }
    m_autoMoveTimer->start(m_vitesseTimer); //timer en ms.
    //hiScoreChanged(m_hiScore);
}

void Board::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    if(m_changementPlateau==1)
    {
        paintTout(painter);
    }

    if (m_autoMoveTimer->isActive())
    {
        if(m_quelPlateau=="")
        {
            paintShot(painter);
        }
        else if(m_quelPlateau=="densite")
        {
            paintDensite(painter);
        }
        else if(m_quelPlateau=="arete")
        {
            paintArete(painter);
        }
        else if(m_quelPlateau=="nonoriente")
        {
            paintNonOriente(painter);
        }
        else if(m_quelPlateau=="pomme")
        {
            paintPomme(painter);
        }
    }
}

void Board::paintTout(QPainter &painter)
{
    //setPalette(QPalette(QColor(250, 250, 200)));
    painter.setPen(Qt::NoPen);
    if(m_quelPlateau=="densite" || m_quelPlateau=="arete" || m_quelPlateau=="nonoriente" || m_quelPlateau=="pomme")
    {
        painter.setBrush(Qt::white);
    }
    else
    {
        painter.setBrush(QColor(250, 250, 200));
    }

    //if(m_quelPlateau="densite")
    //{
    painter.drawRect(m_rectPlateau());
    //}
}

void Board::paintDensite(QPainter &painter)
{
    painter.setPen(Qt::NoPen);

    int minTransp(20);
    int maxTransp(240);
    int pas((maxTransp-minTransp)/(m_valeurMaxGrilleSauvegardee+1));

    for(int y(0); y<m_grilleHauteur; y++)
    {
        for(int x(0); x<m_grilleLongueur; x++)
        {
            painter.setBrush(QColor(0, 0, 255, minTransp+pas*m_grilleSauvegardee[y*m_grilleLongueur+x]));
            painter.drawRect(m_rect(x,y)); //painter.drawEllipse(m_rectTete(0));
        }
       }
}

void Board::paintArete(QPainter &painter)
{
    painter.setPen(Qt::NoPen);

    int minTransp(20);
    int maxTransp(240);
    int pas((maxTransp-minTransp)/(m_valeurMaxAreteSauvegardee+1));

    for(int y(0); y<m_grilleHauteur; y++)
    {
        for(int x(0); x<m_grilleLongueur; x++)
        {
            //painter.setBrush(QColor(0, 255, 0, minTransp+pas*m_aretesEst[y*m_grilleLongueur+x]));
            painter.setBrush(QColor(255, 0, 0, minTransp+pas*m_aretesEst[y*m_grilleLongueur+x]));
            //painter.drawRect(m_rectEst(x,y));
            painter.drawPolygon(m_triangleEst(x,y));

            //painter.setBrush(QColor(0, 255, 0, minTransp+pas*m_aretesNord[y*m_grilleLongueur+x]));
            painter.setBrush(QColor(255, 0, 0, minTransp+pas*m_aretesNord[y*m_grilleLongueur+x]));
            painter.drawPolygon(m_triangleNord(x,y));
            //painter.drawRect(m_rectNord(x,y));

            painter.setBrush(QColor(255, 0, 0, minTransp+pas*m_aretesOuest[y*m_grilleLongueur+x]));
            painter.drawPolygon(m_triangleOuest(x,y));
            //painter.drawRect(m_rectOuest(x,y));

            painter.setBrush(QColor(255, 0, 0, minTransp+pas*m_aretesSud[y*m_grilleLongueur+x]));
            painter.drawPolygon(m_triangleSud(x,y));
            //painter.drawRect(m_rectSud(x,y));
        }
    }
}

void Board::paintNonOriente(QPainter &painter)
{
    painter.setPen(Qt::NoPen);

    int minTransp(20);
    int maxTransp(240);
    int pas((maxTransp-minTransp)/(2*m_valeurMaxAreteSauvegardee));

    for(int y(0); y<m_grilleHauteur; y++)
    {
        for(int x(0); x<m_grilleLongueur; x++)
        {
            if(x!=0)
            {
                painter.setBrush(QColor(255, 0, 0, minTransp+pas*(m_aretesEst[y*m_grilleLongueur+x]+m_aretesOuest[y*m_grilleLongueur+(x-1)])));
                //painter.drawRect(m_rectEst(x,y));
                painter.drawPolygon(m_triangleEst(x,y));

                painter.setBrush(QColor(255, 0, 0, minTransp+pas*(m_aretesEst[y*m_grilleLongueur+x]+m_aretesOuest[y*m_grilleLongueur+(x-1)])));
                painter.drawPolygon(m_triangleOuest(x-1,y));
                //painter.drawRect(m_rectOuest(x,y));
            }
            else
            {
                painter.setBrush(QColor(255, 0, 0, minTransp+pas*(m_aretesEst[y*m_grilleLongueur+x]+m_aretesOuest[y*m_grilleLongueur+(x-1)+m_grilleLongueur])));
                //painter.drawRect(m_rectEst(x,y));
                painter.drawPolygon(m_triangleEst(x,y));

                painter.setBrush(QColor(255, 0, 0, minTransp+pas*(m_aretesEst[y*m_grilleLongueur+x]+m_aretesOuest[y*m_grilleLongueur+(x-1)+m_grilleLongueur])));
                painter.drawPolygon(m_triangleOuest(x-1+m_grilleLongueur,y));
                //painter.drawRect(m_rectOuest(x,y));
            }

            if(y!=0)
            {
                painter.setBrush(QColor(255, 0, 0, minTransp+pas*(m_aretesNord[(y-1)*m_grilleLongueur+x]+m_aretesSud[y*m_grilleLongueur+x])));
                painter.drawPolygon(m_triangleNord(x,y-1));
                //painter.drawRect(m_rectNord(x,y));

                painter.setBrush(QColor(255, 0, 0, minTransp+pas*(m_aretesNord[(y-1)*m_grilleLongueur+x]+m_aretesSud[y*m_grilleLongueur+x])));
                painter.drawPolygon(m_triangleSud(x,y));
                //painter.drawRect(m_rectSud(x,y));
            }
            else
            {
                painter.setBrush(QColor(255, 0, 0, minTransp+pas*(m_aretesNord[(y-1+m_grilleHauteur)*m_grilleLongueur+x]+m_aretesSud[y*m_grilleLongueur+x])));
                painter.drawPolygon(m_triangleNord(x,y-1+m_grilleHauteur));
                //painter.drawRect(m_rectNord(x,y));

                painter.setBrush(QColor(255, 0, 0, minTransp+pas*(m_aretesNord[(y-1+m_grilleHauteur)*m_grilleLongueur+x]+m_aretesSud[y*m_grilleLongueur+x])));
                painter.drawPolygon(m_triangleSud(x,y));
                //painter.drawRect(m_rectSud(x,y));
            }
        }
    }
}

void Board::paintPomme(QPainter &painter)
{
    painter.setPen(Qt::NoPen);
    int minTransp(20);
    int maxTransp(240);
    int pas((maxTransp-minTransp)/(m_valeurMaxGrillePomme+1));
    //std::cout << m_valeurMaxGrillePomme << " est nb pommes" << std::endl;
    for(int y(0); y<m_grilleHauteur; y++)
    {
        for(int x(0); x<m_grilleLongueur; x++)
        {
            //std::cout << x << "	" << y << "	" << m_grillePomme[y*m_grilleLongueur+x] << std::endl;
            painter.setBrush(QColor(0, 255, 0, minTransp+pas*m_grillePomme[y*m_grilleLongueur+x]));
            painter.drawRect(m_rect(x,y)); //painter.drawEllipse(m_rectTete(0));
        }
    }
}

void Board::paintShot(QPainter &painter)
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    if(m_gagner==TRUE)
    {
        painter.setBrush(QColor(60, 60, 200));
    }

    //dessiner corps
    for(unsigned int i(0); i<std::min(m_positionsSauvegardeesXmod.size()-1, m_longueur); i++)
    {
        painter.drawRect(m_rectCorps(m_positionsSauvegardeesXmod.size()-i-2,0));
    }

    //dessiner queue mouvante
    if(m_positionsSauvegardeesXmod.size()>m_longueur+1)
    {
        //painter.setBrush(Qt::blue);
        painter.drawRect(m_rectQueue(0));
    }

    //dessiner tête
    //painter.setBrush(Qt::green);
    painter.drawRect(m_rectTeteBord(0));
    if(m_perdre==TRUE)
    {
        painter.setBrush(Qt::red);
    }
    if(m_gagner==TRUE)
    {
        painter.setBrush(QColor(80, 80, 200));
    }

    painter.drawRect(m_rectTete(0)); //painter.drawEllipse(m_rectTete(0));

    QRect pomme(m_rectPomme(0)); //placé au début, on le déplace dans la ligne suivante
    painter.setPen(Qt::NoPen);

    //painter.setBrush(Qt::green);
    painter.setBrush(QColor(std::min(2*(100-m_pointsEnCours),128), std::max(255-1*(100-m_pointsEnCours),128), std::min(2*(100-m_pointsEnCours),128)));
    //painter.drawEllipse(pomme);
    if(m_gagner==FALSE)
    {
        painter.drawRect(pomme);
    }
}


QRect Board::m_rectTete(bool affiche)
{
    QRect result(0, 0, m_tailleTete, m_tailleTete); //placé au début, on le déplace dans la ligne suivante
    if(m_perdre==FALSE)
    {
        result.moveTopLeft(QPoint(m_positionEnCoursX, m_positionEnCoursY));
    }
    else
    {
        result.moveTopLeft(QPoint(m_positionsSauvegardeesXmod[m_positionsSauvegardeesXmod.size()-2]*m_tailleTete, m_positionsSauvegardeesYmod[m_positionsSauvegardeesXmod.size()-2]*m_tailleTete)); //positionné exactement sur un carré de la grille
    }

    if(affiche==1)
    {
        std::cout << "m_rectTete "; afficher(result);
    }
    return result;
}


QPolygon Board::m_triangleNord(int x, int y)
{
    QPolygon triangleN(3);
    triangleN.setPoint(0, x*m_tailleTete, y*m_tailleTete+m_tailleTete);
    triangleN.setPoint(1, x*m_tailleTete+m_tailleTete, y*m_tailleTete+m_tailleTete);
    triangleN.setPoint(2, x*m_tailleTete+m_tailleTete/2, y*m_tailleTete+m_tailleTete/2);
    return triangleN;
}


QRect Board::m_rectNord(int x, int y)
{
    //int largeur(m_tailleTete/4);
    int largeur(5);
    return QRect(x*m_tailleTete, y*m_tailleTete+m_tailleTete-largeur, m_tailleTete-2*largeur, largeur);
}

QPolygon Board::m_triangleSud(int x, int y)
{
    QPolygon triangleN(3);
    triangleN.setPoint(0, x*m_tailleTete, y*m_tailleTete);
    triangleN.setPoint(1, x*m_tailleTete+m_tailleTete, y*m_tailleTete);
    triangleN.setPoint(2, x*m_tailleTete+m_tailleTete/2, y*m_tailleTete+m_tailleTete/2);
    return triangleN;
}

QRect Board::m_rectSud(int x, int y)
{
    //int largeur(m_tailleTete/4);
    int largeur(5);
    if(y>0)
    {
        return QRect(x*m_tailleTete, y*m_tailleTete-2*largeur, m_tailleTete-2*largeur, largeur);
    }
    else
    {
        return QRect(x*m_tailleTete, y*m_tailleTete-2*largeur+m_grilleHauteur*m_tailleTete, m_tailleTete-2*largeur, largeur);
    }
}

QPolygon Board::m_triangleEst(int x, int y)
{
    QPolygon triangleN(3);
    triangleN.setPoint(0, x*m_tailleTete, y*m_tailleTete);
    triangleN.setPoint(1, x*m_tailleTete, y*m_tailleTete+m_tailleTete);
    triangleN.setPoint(2, x*m_tailleTete+m_tailleTete/2, y*m_tailleTete+m_tailleTete/2);
    return triangleN;
}

QRect Board::m_rectEst(int x, int y)
{
    //int largeur(m_tailleTete/4);
    int largeur(5);
    if(x>0)
    {
        return QRect(x*m_tailleTete-2*largeur, y*m_tailleTete, largeur, m_tailleTete-2*largeur);
    }
    else
    {
        return QRect(x*m_tailleTete-2*largeur+m_grilleLongueur*m_tailleTete, y*m_tailleTete, largeur, m_tailleTete-2*largeur);
    }
}

QPolygon Board::m_triangleOuest(int x, int y)
{
    QPolygon triangleN(3);
    triangleN.setPoint(0, x*m_tailleTete+m_tailleTete, y*m_tailleTete);
    triangleN.setPoint(1, x*m_tailleTete+m_tailleTete, y*m_tailleTete+m_tailleTete);
    triangleN.setPoint(2, x*m_tailleTete+m_tailleTete/2, y*m_tailleTete+m_tailleTete/2);
    return triangleN;
}

QRect Board::m_rectOuest(int x, int y)
{
    //int largeur(m_tailleTete/4);
    int largeur(5);
    return QRect(x*m_tailleTete-largeur+m_tailleTete, y*m_tailleTete, largeur, m_tailleTete-2*largeur);
}

QRect Board::m_rect(int x, int y)
{
    return QRect(x*m_tailleTete, y*m_tailleTete, m_tailleTete, m_tailleTete);
}


QRect Board::m_rectTeteBord(bool affiche)
{
    QRect result;
    bool existe(FALSE); //regarde si ce rectangle est non vide.

    if(m_positionEnCoursY > m_tailleTete*m_grilleHauteur-m_tailleTete)
    {
        result=QRect(0, 0, m_tailleTete, m_tailleTete);
        result.moveTopLeft(QPoint(m_positionEnCoursX, m_positionEnCoursY-m_tailleTete*m_grilleHauteur));
        existe=TRUE;
    }
    else if(m_positionEnCoursX > m_tailleTete*m_grilleLongueur-m_tailleTete)
    {
        result=QRect(0, 0, m_tailleTete, m_tailleTete);
        result.moveTopLeft(QPoint(m_positionEnCoursX-m_tailleTete*m_grilleLongueur, m_positionEnCoursY));
        existe=TRUE;
    }

    if(affiche==1 && existe==TRUE)
    {
        std::cout << "m_rectTeteBord "; afficher(result);
    }
    return result;
}

QRect Board::m_rectCorps(unsigned int i, bool affiche)
{
    if(i > m_positionsSauvegardeesXmod.size())
    {
        std::cout << "ERREUR m_rectCorps" << std::endl;
    }
    QRect result(0, 0, m_tailleTete, m_tailleTete); //placé au début, on le déplace dans la ligne suivante
    result.moveTopLeft(QPoint(m_positionsSauvegardeesXmod[i]*m_tailleTete, m_positionsSauvegardeesYmod[i]*m_tailleTete));

    if(affiche==1)
    {
        std::cout << "m_rectCorps "; afficher(result);
    }
    return result;
}

QRect Board::m_rectQueue(bool affiche)
{
    unsigned int i(m_positionsSauvegardeesX.size()-m_longueur-2);
    if(i > m_positionsSauvegardeesXmod.size())
    {
        std::cout << "ERREUR m_rectQueue" << std::endl;
    }
    QRect result(0, 0, m_tailleTete, m_tailleTete); //placé au début, on le déplace dans la ligne suivante

    int xActuel(m_positionsSauvegardeesXmod[i]*m_tailleTete);
    int yActuel(m_positionsSauvegardeesYmod[i]*m_tailleTete);
    if(m_perdre==TRUE)
    {
        result.moveTopLeft(QPoint(xActuel, yActuel));
        //std::cout << "!!" << xActuel << " " << yActuel << std::endl;
    }
    else if(m_nePasBougerLaQueue != 0)
    {
        //std::cout << "q : " << m_nePasBougerLaQueue << std::endl;
        result.moveTopLeft(QPoint(m_positionsSauvegardeesXmod[i+1]*m_tailleTete, m_positionsSauvegardeesYmod[i+1]*m_tailleTete));
    }
    else
    {
        int ajout(0);
        if(m_directions.back()=="est")
        {
            ajout=m_positionEnCoursX % m_tailleTete;
        }
        else if(m_directions.back()=="ouest")
        {
            ajout=m_tailleTete - m_positionEnCoursX % m_tailleTete;
        }
        else if(m_directions.back()=="nord")
        {
            ajout=m_tailleTete - m_positionEnCoursY % m_tailleTete;
        }
        else if(m_directions.back()=="sud")
        {
            ajout=m_positionEnCoursY % m_tailleTete;
        }

        std::string direction(m_directions[m_positionsSauvegardeesX.size()-m_longueur-2]);
        int signe(0); //1 ou -1
        if(direction == "ouest")
        {
            signe = -1;
            result.moveTopLeft(QPoint(xActuel+signe*ajout, yActuel));
        }
        else if(direction == "nord")
        {
            signe = -1;
            result.moveTopLeft(QPoint(xActuel, yActuel+signe*ajout));
        }
        else if(direction == "est")
        {
            signe = 1;
            result.moveTopLeft(QPoint(xActuel+signe*ajout, yActuel));
        }
        else if(direction == "sud")
        {
            signe = 1;
            result.moveTopLeft(QPoint(xActuel, yActuel+signe*ajout));
        }
    }

    if(affiche==1)
    {
        std::cout << "m_rectQueue "; afficher(result);
    }
    return result;
}

void Board::afficher(QRect result) const
{
    std::cout << "(" << result.x() << "," << result.y() << ")"<< std::endl;
}

void Board::updateMove()
{
    if(m_nePasBougerLaQueue != 0)
    {
        m_nePasBougerLaQueue--;
    }

    //update tête
    QRegion region = m_rectTete(0); //cf après update ce n'est plus un rectangle mais une union de deux rectangles
    region = region.united(m_rectTeteBord(0));

    //update queue mouvante
    if(m_positionsSauvegardeesXmod.size() > m_longueur+1)
    {
        region = region.united(m_rectQueue(0));
    }

    //update corps juste avant la queue (cf il se peut que un pixel soit mal updaté)
    if(m_positionsSauvegardeesXmod.size() >= m_longueur+1)
    {
        region = region.united(m_rectCorps(m_positionsSauvegardeesXmod.size()-(m_longueur+1),0));
    }

    if(m_perdre==FALSE)
    {
        avancer();
    }

    region = region.united(m_rectTete(0));
    region = region.united(m_rectTeteBord(0));

    update(region); //This will send a single paint event for just the one or two rectangles that need updating //faire le moins de update possible car ralentit beaucoup.

    //update pomme
    update(m_rectPomme(0));
    if(m_positionsSauvegardeesPommeX.size() > 1)
    {
        update(QRect(m_tailleTete*m_positionsSauvegardeesPommeX[m_positionsSauvegardeesPommeX.size()-2], m_tailleTete*m_positionsSauvegardeesPommeY[m_positionsSauvegardeesPommeY.size()-2], m_tailleTete, m_tailleTete));
    }

    if(m_changementPlateau==1)
    {
        update(m_rectPlateau());
        m_changementPlateau=0;
    }
}

QRect Board::m_rectPlateau()
{
    return QRect(-1,-1,m_tailleTete*m_grilleLongueur+1,m_tailleTete*m_grilleHauteur+1);
}

QRect Board::m_rectPomme(bool affiche)
{
    //QRect result(m_positionPommeX + m_tailleTete/4, m_positionPommeY + m_tailleTete/4, m_tailleTete/2, m_tailleTete/2);
    QRect result(m_positionPommeX, m_positionPommeY, m_tailleTete, m_tailleTete);

    if(affiche==1)
    {
        std::cout << "m_rectPomme "; afficher(result);
    }
    return result;
}

void Board::updateSeulement()
{
    //update tête
    QRegion region = m_rectTete(0); //cf après update ce n'est plus un rectangle mais une union de deux rectangles
    region = region.united(m_rectTeteBord(0));

    //update queue mouvante
    if(m_positionsSauvegardeesXmod.size() > m_longueur+1)
    {
        region = region.united(m_rectQueue(0));
    }

    //update corps juste avant la queue (cf il se peut que un pixel soit mal updaté)
    unsigned int taille(m_positionsSauvegardeesXmod.size()-1);
    for(unsigned int i(0); i<std::min(taille, m_longueur); i++)
    {
        region = region.united(m_rectCorps(taille-i-1,0));
    }

    region = region.united(m_rectTete(0));
    region = region.united(m_rectTeteBord(0));
    update(region); //This will send a single paint event for just the one or two rectangles that need updating //faire le moins de update possible car ralentit beaucoup.
}

void Board::agrandissementSnake()
{
    if(m_nbPasAvantAgrandissementSnake.size() != 0)
    {
        //std::cout << "... ";
        for(unsigned int i(0); i<m_nbPasAvantAgrandissementSnake.size(); i++)
        {
            //std::cout << m_nbPasAvantAgrandissementSnake[i] << " ";
            if(m_nbPasAvantAgrandissementSnake[i]!=0)
            {
                m_nbPasAvantAgrandissementSnake[i]--;
            }
            else
            {
                std::cout << "ERREUR Board::agrandissementSnake() deux augmentations de longueur en même temps." << std::endl;
            }
        }
        //std::cout << std::endl;

        if(m_nbPasAvantAgrandissementSnake[0]==0) //cf c'est forcément lui qui sonne le premier
        {
            m_nbPasAvantAgrandissementSnake.pop_front();
            m_longueur+=1;
            updateSeulement();
            m_nePasBougerLaQueue+=m_tailleTete;
        }
    }
}

void Board::decroitPointEnCours()
{
    //std::cout << m_nbPasAvantDecroissancePoints << std::endl;
    if(m_pointsEnCours!=1 && m_nbPasAvantDecroissancePoints==0)
    {
        m_pointsEnCours=0.95*m_pointsEnCours;
    }
    //std::cout << m_pointsEnCours << " sont les points."<< std::endl;
}

void Board::avancer()
{
    if(m_positionEnCoursX % m_tailleTete == 0 && m_positionEnCoursY % m_tailleTete == 0) //on est sur la grille exactement
    {
        m_temps++;
        //std::cout << m_temps << " c'est le temps " << std::endl;
        if(m_nbPasAvantDecroissancePoints!=0)
        {
            m_nbPasAvantDecroissancePoints--;
        }
        agrandissementSnake();
        //collisionAvecPomme();
        sauvegardePositions(); //ajout data
        sauvegardePositionsModulo(); //ajout data modulo le plateau
        sauvegardeGrille();
        sauvegardeArete();
        //AFFICHAGE
        //std::cout << m_directions.back();
        //std::cout << "	(" << m_positionsSauvegardeesX.back() << "," << m_positionsSauvegardeesY.back() << ")";
        //std::cout << "	(" << m_positionsSauvegardeesXmod.back() << "," << m_positionsSauvegardeesYmod.back() << ")" << std::endl;

        //mesureDistancePommeTete();
        //testCollision();
        decroitPointEnCours();

        if(collisionAvecPomme())
        {
            pommeMangee();
        }

        sauvegardeTempsPointsNbPommesMangeesLongueurSnake();
        if(m_score==3000)
        {
            gagner();
        }
    }

    if(m_positionEnCoursX % m_tailleTete == 1 || m_positionEnCoursY % m_tailleTete == 1 || m_positionEnCoursX % m_tailleTete == m_tailleTete-1 || m_positionEnCoursY % m_tailleTete == m_tailleTete-1) //on vient de quitter la griller
    {
        testCollision();
    }

    //lié à ancienneDirection (pour éviter des mouvements du snake)
    if( m_repositionnerSurLaGrille==FALSE || ( m_positionEnCoursX % m_tailleTete == 0 && m_positionEnCoursY % m_tailleTete == 0) )
    {
        m_ancienneDirection=m_direction;
        m_repositionnerSurLaGrille=FALSE;
    }

    //on avance d'un pas (modulo le plateau)
    avancerUnPas();
}

void Board::testCollision()
{
    //test la collision avec les valeurs précédentes
    for(unsigned int i(0); i<std::min(m_longueur,m_positionsSauvegardeesXmod.size()-1); i++)
    {
        unsigned int positioniPasAvant(m_positionsSauvegardeesXmod.size()-2-i);
        if(m_positionsSauvegardeesXmod.back()==m_positionsSauvegardeesXmod[positioniPasAvant] && m_positionsSauvegardeesYmod.back()==m_positionsSauvegardeesYmod[positioniPasAvant])
        {
            perdre();
        }
    }

    //dans les cas où m_longueur est impaire (donc le snake totale est de taille 4, 6, 8 etc.), test de collision réelle (qui peut survenir ou ne pas survenir suivant les configurations)
    if(m_positionsSauvegardeesXmod.size() >= m_longueur+2)
    {
        unsigned int positionQueue(m_positionsSauvegardeesXmod.size()-m_longueur-2);
        if(m_positionsSauvegardeesXmod.back()==m_positionsSauvegardeesXmod[positionQueue] && m_positionsSauvegardeesYmod.back()==m_positionsSauvegardeesYmod[positionQueue]) //s'il y a une possibilité de collision (cas ambigü)
        {
            if(m_rectTete(0).intersects(m_rectQueue(0)) == 1 || m_rectTeteBord(0).intersects(m_rectQueue(0)) == 1) //on fait le test avec les rectangles
            {
                perdre();
            }
        }
    }
}

bool Board::collisionAvecPomme()
{
    if(m_positionsSauvegardeesPommeX.back()==m_positionsSauvegardeesXmod.back() && m_positionsSauvegardeesPommeY.back()==m_positionsSauvegardeesYmod.back())
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void Board::pommeMangee()
{
    //AFFICHAGE
    //std::cout << "pomme miam" << std::endl;
    augmenterLongueur();
    nouvellePositionPomme();
    m_nbPasAvantDecroissancePoints=mesureDistancePommeTete();

    //AFFICHAGE
    //std::cout << "Nb de pommes mangées : " << m_positionsSauvegardeesPommeX.size()-1 << std::endl;

    if(m_score+m_pointsEnCours<3000)
    {
        m_score+=m_pointsEnCours;
        m_pointsEnCours=100;
        //AFFICHAGE
        //std::cout << "	Le score est :" << m_score << "!" << std::endl;
        scoreChanged(m_score);
    }
    else
    {
        m_score=3000;
    }
}

void Board::sauvegardePositions()
{
    m_directions.push_back(convertirDirection(m_direction).c_str());
    if(m_direction==0)
    {
        m_positionsSauvegardeesX.push_back(m_positionsSauvegardeesX.back()+1);
        m_positionsSauvegardeesY.push_back(m_positionsSauvegardeesY.back());
    }
    if(m_direction==1)
    {
        m_positionsSauvegardeesX.push_back(m_positionsSauvegardeesX.back());
        m_positionsSauvegardeesY.push_back(m_positionsSauvegardeesY.back()-1);
    }
    if(m_direction==2)
    {
        m_positionsSauvegardeesX.push_back(m_positionsSauvegardeesX.back()-1);
        m_positionsSauvegardeesY.push_back(m_positionsSauvegardeesY.back());
    }
    if(m_direction==3)
    {
        m_positionsSauvegardeesX.push_back(m_positionsSauvegardeesX.back());
        m_positionsSauvegardeesY.push_back(m_positionsSauvegardeesY.back()+1);
    }
}

void Board::sauvegardeTempsPointsNbPommesMangeesLongueurSnake()
{
    m_tempsSave.push_back(m_temps);
    m_scoreSave.push_back(m_score);
    m_nbPommesMangeesSave.push_back(m_positionsSauvegardeesPommeX.size()-1);
    m_longueurSnakeSave.push_back(m_longueur+1);

    m_positionsSauvegardeesPommeXcomplet.push_back(m_positionsSauvegardeesPommeX.back());
    m_positionsSauvegardeesPommeYcomplet.push_back(m_positionsSauvegardeesPommeY.back());
}

void Board::sauvegardePositionsModulo()
{
    m_positionsSauvegardeesXmod.push_back(m_positionsSauvegardeesX.back() % m_grilleLongueur);
    m_positionsSauvegardeesYmod.push_back(m_positionsSauvegardeesY.back() % m_grilleHauteur);
    if(m_positionsSauvegardeesXmod.back()<0)
    {
        m_positionsSauvegardeesXmod.back()=m_positionsSauvegardeesXmod.back()+m_grilleLongueur;
    }
    if(m_positionsSauvegardeesYmod.back()<0)
    {
        m_positionsSauvegardeesYmod.back()=m_positionsSauvegardeesYmod.back()+m_grilleHauteur;
    }
}

void Board::sauvegardeArete()
{
    int xAvant(m_positionsSauvegardeesXmod.back());
    int yAvant(m_positionsSauvegardeesYmod.back());

    //int xAvantAvant(m_positionsSauvegardeesXmod[m_positionsSauvegardeesXmod.size()-2]);
    //int yAvantAvant(m_positionsSauvegardeesYmod[m_positionsSauvegardeesYmod.size()-2]);
    int aTesterValeur(-1);

    if(m_direction==0)
    {
        m_aretesEst[m_grilleLongueur*yAvant+xAvant]+=1;
        aTesterValeur=m_aretesEst[m_grilleLongueur*yAvant+xAvant];
        //std::cout << xAvant << " " << yAvant << " " << "est" << " " << m_aretesEst[m_grilleLongueur*yAvant+xAvant] << std::endl;
    }
    else if(m_direction==1)
    {
        m_aretesNord[m_grilleLongueur*yAvant+xAvant]+=1;
        aTesterValeur=m_aretesNord[m_grilleLongueur*yAvant+xAvant];
        //std::cout << xAvant << " " << yAvant << " " << "nord" << " " << m_aretesNord[m_grilleLongueur*yAvant+xAvant] << std::endl;
    }
    else if(m_direction==2)
    {
        m_aretesOuest[m_grilleLongueur*yAvant+xAvant]+=1;
        aTesterValeur=m_aretesOuest[m_grilleLongueur*yAvant+xAvant];
        //std::cout << xAvant << " " << yAvant << " " << "ouest" << " " << m_aretesOuest[m_grilleLongueur*yAvant+xAvant] << std::endl;
    }
    else if(m_direction==3)
    {
        m_aretesSud[m_grilleLongueur*yAvant+xAvant]+=1;
        aTesterValeur=m_aretesSud[m_grilleLongueur*yAvant+xAvant];
        //std::cout << xAvant << " " << yAvant << " " << "sud" << " " << m_aretesSud[m_grilleLongueur*yAvant+xAvant] << std::endl;
    }

    //std::cout << m_valeurMaxAreteSauvegardee << std::endl;
    if(m_valeurMaxAreteSauvegardee<aTesterValeur)
    {
        m_valeurMaxAreteSauvegardee=aTesterValeur;
        if(m_quelPlateau=="arete" || m_quelPlateau=="nonoriente")
        {
            m_changementPlateau=1;
        }
    }

    //std::cout << m_valeurMaxAreteSauvegardee << std::endl;
}

void Board::sauvegardeGrille()
{
    int xAvant(m_positionsSauvegardeesXmod.back());
    int yAvant(m_positionsSauvegardeesYmod.back());
    m_grilleSauvegardee[m_grilleLongueur*yAvant+xAvant]+=1;
    if(m_valeurMaxGrilleSauvegardee<m_grilleSauvegardee[m_grilleLongueur*yAvant+xAvant])
    {
        m_valeurMaxGrilleSauvegardee=m_grilleSauvegardee[m_grilleLongueur*yAvant+xAvant];
        if(m_quelPlateau=="densite")
        {
            m_changementPlateau=1;
        }
    }
    //std::cout << xAvant << " " << yAvant << " " << m_grilleSauvegardee[m_grilleLongueur*yAvant+xAvant] << std::endl;
}

void Board::avancerUnPas()
{
    if(m_ancienneDirection==0)
    {
        m_positionEnCoursX++;
    }
    if(m_ancienneDirection==1)
    {
        m_positionEnCoursY--;
    }
    if(m_ancienneDirection==2)
    {
        m_positionEnCoursX--;
    }
    if(m_ancienneDirection==3)
    {
        m_positionEnCoursY++;
    }

    if(m_positionEnCoursY < 0)
    {
        m_positionEnCoursY += m_tailleTete*m_grilleHauteur;
    }
    if(m_positionEnCoursY >= m_tailleTete*m_grilleHauteur)
    {
        m_positionEnCoursY -= m_tailleTete*m_grilleHauteur;
    }
    if(m_positionEnCoursX < 0)
    {
        m_positionEnCoursX += m_tailleTete*m_grilleLongueur;
    }
    if(m_positionEnCoursX >= m_tailleTete*m_grilleLongueur)
    {
        m_positionEnCoursX -= m_tailleTete*m_grilleLongueur;
    }
}

std::string Board::convertirDirection(int direction)
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

void Board::ecrire()
{
    //AFFICHAGE
    //std::cout << m_score << " score final" << std::endl;
    QFile data("output.txt");
    if(data.open(QFile::WriteOnly | QIODevice::Append))
    {
        QTextStream out(&data);
        //out << "9999 9999" << endl; //endl de la classe QTextStream, pas de std.

        //std::cout << m_tempsSave.size() << " " << m_scoreSave.size() << " " << m_nbPommesMangeesSave.size() << " " << m_longueurSnakeSave.size() << " " <<  m_positionsSauvegardeesX.size() << " " << m_positionsSauvegardeesY.size() << " " << m_positionsSauvegardeesPommeXcomplet.size() << " " << m_positionsSauvegardeesPommeYcomplet.size() << std::endl;

        if(m_partieEnCours==0)
        {
            out << "partieEnCours" << "	" << "temps" << "	" << "score" << "	" << "nbPommesMangees" << "	" << "longueurSnake" << "	" <<  "positionsX" << "	" << "positionsY" << "	" << "positionsPommeX" << "	" << "positionsPommeY" << endl;
        }

        for(unsigned int i(0); i< m_positionsSauvegardeesX.size(); i++)
        {
            out << m_partieEnCours << "	" << m_tempsSave[i] << "	" << m_scoreSave[i] << "	" << m_nbPommesMangeesSave[i] << "	" << m_longueurSnakeSave[i] << "	" <<  m_positionsSauvegardeesX[i] << "	" << m_positionsSauvegardeesY[i] << "	" << m_positionsSauvegardeesPommeXcomplet[i] << "	" << m_positionsSauvegardeesPommeYcomplet[i] << endl;
            //out << m_partieEnCours << " " << m_tempsSave[i] << " " << m_scoreSave[i] << " " << m_nbPommesMangeesSave[i] << " " << m_longueurSnakeSave[i] << " " <<  m_positionsSauvegardeesX[i] << " " << m_positionsSauvegardeesY[i] << endl;
        }
    }

    if(m_hiScore < m_score)
    {
        //AFFICHAGE
        //std::cout << m_score << " score final" << std::endl;
        QFile data2("hiscore.txt");
        if(data2.open(QFile::WriteOnly))
        {
            QTextStream out2(&data2);
            out2 << base(m_score,9) << "	" << base(m_score,7) << "	" << base(m_score,8);
        }
        /*
          std::string test = "45";
          int myint = std::stoi(test);
          std::cout << myint << '\n';
        */
        //std::cout << base(m_score,2) << std::endl;
    }
}

void Board::ecrireNom(char k)
{
    //AFFICHAGE
    //std::cout << m_score << " score final" << std::endl;
    QFile data("outputNoms.txt");
    if(data.open(QFile::WriteOnly | QIODevice::Append))
    {
        QTextStream out(&data);
       out << m_partieEnCours << "	" << k << endl;
    }
}

int Board::base(int nombre, int base)
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

int Board::lirePartieEnCours()
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

int Board::lireHiScore()
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

void Board::augmenterLongueur()
{
    m_nbPasAvantAgrandissementSnake.push_back(m_longueur+1);
    //updateSeulement();
}

void Board::perdre()
{
    //AFFICHAGE
    //std::cout << "BOOM !" << std::endl;
    m_perdre=TRUE;
    ecrire();
    //m_quelPlateau="densite";
}

void Board::gagner()
{
    //AFFICHAGE
    //std::cout << "GAGNE !" << std::endl;
    m_gagner=TRUE;
    m_perdre=TRUE;
    m_changementPlateau=1;
    if(m_temps <= 1000 && (m_positionsSauvegardeesPommeX.size()-1) <= 100)
    {
        m_score+=10*(1000-m_temps)+100*(100-(m_positionsSauvegardeesPommeX.size()-1));
    }
    else if(m_temps <= 1000 && (m_positionsSauvegardeesPommeX.size()-1) > 100)
    {
        m_score+=10*(1000-m_temps);
    }
    else if(m_temps > 1000 && (m_positionsSauvegardeesPommeX.size()-1) <= 100)
    {
        m_score+=100*(100-(m_positionsSauvegardeesPommeX.size()-1));
    }
    scoreChanged(m_score);
    ecrire();
}

void Board::nouvellePositionPomme()
{
    //int X(qrand() % m_grilleLongueur);
    //int Y(qrand() % m_grilleHauteur);
    int X(-1);
    int Y(-1);
    while(X==-1)
    {
        int nouveauX(Alea::rand_integer(m_grilleLongueur));
        int nouveauY(Alea::rand_integer(m_grilleHauteur));
        //std::cout << nouveauX << " " << nouveauY << std::endl;
        if(pasDeCollisionAvecSerpent(nouveauX,nouveauY) || m_perdre == TRUE)
        {
            X=nouveauX;
            Y=nouveauY;
        }
    }

    if(m_perdre==FALSE)
    {
        m_positionPommeX = m_tailleTete*X;
        m_positionPommeY = m_tailleTete*Y;
        m_positionsSauvegardeesPommeX.push_back(X);
        m_positionsSauvegardeesPommeY.push_back(Y);
        //std::cout << "Pomme : " <<  m_positionPommeX << " " << m_positionPommeY << std::endl;

        sauvegardePomme();
    }
}

bool Board::pasDeCollisionAvecSerpent(int nouveauX, int nouveauY)
{
    if((int) m_longueur+1 == m_grilleLongueur*m_grilleHauteur)
    {
        sauvegardePositions(); //enregistre position en cours dans m_positionsSauvegardeesX/Y
        sauvegardePositionsModulo(); //idem modulo la taille du plateau (méthode à lancer après sauvegardePositions())
        gagner();
        return 1;
    }

    for(unsigned int i(0); i<m_longueur+1; i++)
    {
        //std::cout << "test avec : " << m_positionsSauvegardeesXmod[m_positionsSauvegardeesXmod.size()-1-i] << " " << m_positionsSauvegardeesYmod[m_positionsSauvegardeesYmod.size()-1-i] << std::endl;
        if(m_positionsSauvegardeesXmod[m_positionsSauvegardeesXmod.size()-1-i]==nouveauX && m_positionsSauvegardeesYmod[m_positionsSauvegardeesYmod.size()-1-i]==nouveauY)
        {
            return 0;
        }
    }
    //std::cout << "OK" << std::endl;
    return 1;
}

void Board::sauvegardePomme()
{
    int xAvant(m_positionsSauvegardeesPommeX.back());
    int yAvant(m_positionsSauvegardeesPommeY.back());

    m_grillePomme[m_grilleLongueur*yAvant+xAvant]+=1;

    if(m_valeurMaxGrillePomme<m_grillePomme[m_grilleLongueur*yAvant+xAvant])
    {
        m_valeurMaxGrillePomme=m_grillePomme[m_grilleLongueur*yAvant+xAvant];
        if(m_quelPlateau=="pomme")
        {
            m_changementPlateau=1;
        }
    }
    //std::cout << "pommy " << xAvant << " " << yAvant << " " <<  m_grillePomme[m_grilleLongueur*yAvant+xAvant] << std::endl;
}

int Board::mesureDistancePommeTete()
{
    int pommeX(m_positionPommeX/m_tailleTete);
    int pommeY(m_positionPommeY/m_tailleTete);

    int teteX(m_positionEnCoursX/m_tailleTete);
    int teteY(m_positionEnCoursY/m_tailleTete);

    int ecartX(std::min(abs(teteX-pommeX), m_grilleLongueur-abs(teteX-pommeX)));
    int ecartY(std::min(abs(teteY-pommeY), m_grilleHauteur-abs(teteY-pommeY)));

    //AFFICHAGE
    //std::cout << ecartX << " " << ecartY << std::endl;
    return ecartX+ecartY;
}
