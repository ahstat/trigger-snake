#include "Board.h"

/*
  Public methods
*/
Board::Board(QWidget *parent) : QWidget(parent)
{
    // Real-time moving timer. Timer begin when begin() is called
    m_autoMoveTimer = new QTimer(this);

    // We'll move on every time the timer times out
    connect(m_autoMoveTimer, SIGNAL(timeout()), this, SLOT(updateMove()));

    // Initialize variables to default
    newGame();
}

int Board::getHiScore()
{
    return m_hiScore;
}

/*
  Slot
*/
// There is only one slot, which continuously waits for user actions and
// board updates. It is called by newgame().
void Board::updateMove()
{
    // Update the whole board
    update(m_rectPlateau());
    // Note: Doing too many updates will slow down the game

    if(m_endOfGame == FALSE)
    {
        // Motion of the snake
        motion();
    }
}

/*
  Private methods
*/
void Board::newGame()
{
    ////
    // General characteristics
    ////
    m_timerSpeed = 3; // timer in milliseconds -- let to 3 for true speed
    m_nbSquaresX = 16;
    m_nbSquaresY = 8;
    m_cellSize = 48; // size in pixels -- better when m_cellSize == 0 mod 4
    m_score = 0;
    scoreChanged(m_score); // give signal to LCDRange
    m_hiScore = Helpers::lireHiScore(); // retrive hi-score from txt
    hiScoreChanged(m_hiScore); // give signal to LCDRange
    m_currentPlayId = Helpers::lirePartieEnCours();
    m_endOfGame = FALSE;
    m_win = FALSE;
    m_steps = 0;

    ////
    // Information on the snake
    ////
    m_direction = 0;
    m_previousDirection = -1;
    m_repositionOnGrid = FALSE;
    m_currentXPosition = m_cellSize * Alea::rand_integer(m_nbSquaresX);
    m_currentYPosition = m_cellSize * Alea::rand_integer(m_nbSquaresY);
    m_length = 1;

    ////
    // Relations between apple and snake
    ////
    m_pointsCurrentApple = 100;
    m_nbStepsBeforeDecreasingPoints = mesureDistancePommeTete();



    ////
    // Saving all states of the game
    ////
























    //informations sauvegardées du snake (pour les datas)
    m_positionsSauvegardeesX.resize(0);
    m_positionsSauvegardeesY.resize(0);
    m_positionsSauvegardeesX.push_back(m_currentXPosition / m_cellSize);
    m_positionsSauvegardeesY.push_back(m_currentYPosition/m_cellSize); //cf initialement, position en (0,0) sur la grille
    m_positionsSauvegardeesXmod.resize(0);
    m_positionsSauvegardeesYmod.resize(0);
    m_positionsSauvegardeesXmod.push_back(m_currentXPosition/m_cellSize);
    m_positionsSauvegardeesYmod.push_back(m_currentYPosition/m_cellSize); //cf initialement, position en (0,0) sur la grille
    m_directions.resize(0);
    m_grilleSauvegardee.resize(m_nbSquaresX*m_nbSquaresY);
    m_grillePomme.resize(m_nbSquaresX*m_nbSquaresY);
    m_aretesEst.resize(m_nbSquaresX*m_nbSquaresY);
    m_aretesNord.resize(m_nbSquaresX*m_nbSquaresY);
    m_aretesOuest.resize(m_nbSquaresX*m_nbSquaresY);
    m_aretesSud.resize(m_nbSquaresX*m_nbSquaresY);

    for(unsigned int i(0); i < m_grilleSauvegardee.size(); i++)
    {
        m_grilleSauvegardee[i] = 0;
        m_grillePomme[i] = 0;
        m_aretesEst[i] = 0;
        m_aretesNord[i] = 0;
        m_aretesOuest[i] = 0;
        m_aretesSud[i] = 0;
    }


    setPalette(QPalette(QColor(250, 250, 200)));
    setAutoFillBackground(true);
    setFixedSize(m_cellSize * m_nbSquaresX, m_cellSize * m_nbSquaresY);




    sauvegardeGrille();
    //AFFICHAGE
    //std::cout << "init";
    //std::cout << "	(" << m_positionsSauvegardeesX.back() << "," << m_positionsSauvegardeesY.back() << ")";
    //std::cout << "	(" << m_positionsSauvegardeesXmod.back() << "," << m_positionsSauvegardeesYmod.back() << ")" << std::endl;
    m_positionsSauvegardeesPommeX.resize(0);
    m_positionsSauvegardeesPommeY.resize(0);


    m_nbPasAvantAgrandissementSnake.resize(0);
    m_nePasBougerLaQueue = 0;


    nouvellePositionPomme();

    m_changementPlateau = 0;
    m_quelPlateau = "";
    m_valeurMaxGrilleSauvegardee = 0;
    m_valeurMaxAreteSauvegardee = 0;



    m_valeurMaxGrillePomme = 0;








    m_stepsSave.resize(0);
    m_scoreSave.resize(0);
    m_nbPommesMangeesSave.resize(0);
    m_lengthSnakeSave.resize(0);
    m_stepsSave.push_back(0);
    m_scoreSave.push_back(0);
    m_nbPommesMangeesSave.push_back(0);
    m_lengthSnakeSave.push_back(m_length+1);

    m_positionsSauvegardeesPommeXcomplet.resize(0);
    m_positionsSauvegardeesPommeYcomplet.resize(0);
    m_positionsSauvegardeesPommeXcomplet.push_back(m_positionsSauvegardeesPommeX.back());
    m_positionsSauvegardeesPommeYcomplet.push_back(m_positionsSauvegardeesPommeY.back());


    //ce qui change par rapport au constructeur
    m_changementPlateau = 1;
    updateMove();

    m_autoMoveTimer -> stop();
}










QRect Board::m_rectPlateau()
{
    return QRect(-1, -1, m_cellSize*m_nbSquaresX + 1, m_cellSize*m_nbSquaresY + 1);
}










/*
  Regions, rectangles and triangles
*/
QRect Board::m_rect(int x, int y)
{
    return QRect(x, y, m_cellSize, m_cellSize);
}

QPolygon Board::m_triangleNord(int x, int y)
{
    QPolygon triangleN(3);
    triangleN.setPoint(0, x*m_cellSize, y*m_cellSize+m_cellSize);
    triangleN.setPoint(1, x*m_cellSize+m_cellSize, y*m_cellSize+m_cellSize);
    triangleN.setPoint(2, x*m_cellSize+m_cellSize/2, y*m_cellSize+m_cellSize/2);
    return triangleN;
}

QPolygon Board::m_triangleSud(int x, int y)
{
    QPolygon triangleN(3);
    triangleN.setPoint(0, x*m_cellSize, y*m_cellSize);
    triangleN.setPoint(1, x*m_cellSize+m_cellSize, y*m_cellSize);
    triangleN.setPoint(2, x*m_cellSize+m_cellSize/2, y*m_cellSize+m_cellSize/2);
    return triangleN;
}

QPolygon Board::m_triangleEst(int x, int y)
{
    QPolygon triangleN(3);
    triangleN.setPoint(0, x*m_cellSize, y*m_cellSize);
    triangleN.setPoint(1, x*m_cellSize, y*m_cellSize+m_cellSize);
    triangleN.setPoint(2, x*m_cellSize+m_cellSize/2, y*m_cellSize+m_cellSize/2);
    return triangleN;
}

QPolygon Board::m_triangleOuest(int x, int y)
{
    QPolygon triangleN(3);
    triangleN.setPoint(0, x*m_cellSize+m_cellSize, y*m_cellSize);
    triangleN.setPoint(1, x*m_cellSize+m_cellSize, y*m_cellSize+m_cellSize);
    triangleN.setPoint(2, x*m_cellSize+m_cellSize/2, y*m_cellSize+m_cellSize/2);
    return triangleN;
}

QRegion Board::m_rectHead()
{
    QRegion region = QRegion();
    region = region.united(m_rectHeadUsual(0));
    region = region.united(m_rectHeadCorner(0));
    return region;
}

QRect Board::m_rectHeadUsual(bool affiche)
{
    QRect result(m_rect(m_currentXPosition, m_currentYPosition));
    if(m_endOfGame == TRUE)
    {
        // Position of snake's head is took from saved positions
        int indexBeforeBump = m_positionsSauvegardeesXmod.size() - 2;
        int cellXBeforeBump = m_positionsSauvegardeesXmod[indexBeforeBump];
        int cellYBeforeBump = m_positionsSauvegardeesYmod[indexBeforeBump];
        // Move to correct position in pixels
        result.moveTopLeft(QPoint(cellXBeforeBump * m_cellSize, cellYBeforeBump * m_cellSize));
    }

    afficher(affiche, "m_rectHeadUsual", result);
    return result;
}

QRect Board::m_rectHeadCorner(bool affiche)
{
    if(m_currentYPosition > m_cellSize*m_nbSquaresY - m_cellSize)
    {
        QRect result(m_rect(m_currentXPosition, m_currentYPosition - m_cellSize*m_nbSquaresY));
        afficher(affiche, "m_rectHeadCorner Y", result);
        return result;
    }
    else if(m_currentXPosition > m_cellSize*m_nbSquaresX - m_cellSize)
    {
        QRect result(m_rect(m_currentXPosition - m_cellSize*m_nbSquaresX, m_currentYPosition));
        afficher(affiche, "m_rectHeadCorner X", result);
        return result;
    }
    else
    {
        return QRect();
    }
}

QRect Board::m_rectCorps(unsigned int i, bool affiche)
{
    int cellX = m_positionsSauvegardeesXmod[i];
    int cellY = m_positionsSauvegardeesYmod[i];
    QRect result(m_rect(cellX*m_cellSize, cellY*m_cellSize));

    afficher(affiche, "m_rectCorps", result);
    return result;
}

QRect Board::m_rectQueue(bool affiche)
{
    unsigned int i(m_positionsSauvegardeesX.size() - m_length - 2);
    QRect result = QRect();

    std::cout << m_currentXPosition << std::endl;

    if(m_endOfGame == TRUE)
    {
        int cellX(m_positionsSauvegardeesXmod[i]);
        int cellY(m_positionsSauvegardeesYmod[i]);
        result = m_rect(cellX*m_cellSize, cellY*m_cellSize);
    }
    else if(m_nePasBougerLaQueue != 0)
    {
        int cellX(m_positionsSauvegardeesXmod[i+1]);
        int cellY(m_positionsSauvegardeesYmod[i+1]);
        result = m_rect(cellX*m_cellSize, cellY*m_cellSize);
    }
    else
    {
        int cellX(m_positionsSauvegardeesXmod[i]);
        int cellY(m_positionsSauvegardeesYmod[i]);

        int ajout(0);
        if(m_directions.back() == "est")
        {
            //ajout = m_currentXPosition % m_cellSize;
            ajout = (m_currentXPosition + m_cellSize - 1) % m_cellSize;// + 1;
        }
        else if(m_directions.back() == "ouest")
        {
            ajout = m_cellSize - m_currentXPosition % m_cellSize;
        }
        else if(m_directions.back() == "nord")
        {
            ajout = m_cellSize - m_currentYPosition % m_cellSize;
        }
        else if(m_directions.back() == "sud")
        {
            ajout = (m_currentYPosition+m_cellSize - 1) % m_cellSize;// + 1;
        }

        std::string direction(m_directions[m_positionsSauvegardeesX.size() - m_length - 2]);
        int signe(0); //1 or -1
        if(direction == "ouest")
        {
            signe = -1;
            result = m_rect(cellX*m_cellSize+signe*ajout, cellY*m_cellSize);
        }
        else if(direction == "nord")
        {
            signe = -1;
            result = m_rect(cellX*m_cellSize, cellY*m_cellSize+signe*ajout);
        }
        else if(direction == "est")
        {
            signe = 1;
            result = m_rect(cellX*m_cellSize+signe*ajout, cellY*m_cellSize);
        }
        else if(direction == "sud")
        {
            signe = 1;
            result = m_rect(cellX*m_cellSize, cellY*m_cellSize+signe*ajout);
        }

        /*
        //result = m_rect(cellX*m_cellSize+signe*ajout, cellY*m_cellSize);
        std::cout << ajout << std::endl;
        std::cout << m_cellSize << std::endl;
        ajout = (m_currentXPosition - 1) % m_cellSize + 1;
        result = m_rect(cellX*m_cellSize+1*ajout, cellY*m_cellSize);
        */

    }


    afficher(affiche, "m_rectQueue", result);
    return result;
}

QRect Board::m_rectPomme(bool affiche)
{
    QRect result(m_rect(m_positionPommeX, m_positionPommeY));

    afficher(affiche, "m_rectPomme", result);
    return result;
}



void Board::afficher(bool affiche, std::string objName, QRect result) const
{
    if(affiche == TRUE)
    {
        std::cout << objName;
        std::cout << " (" << result.x() << "," << result.y() << ")" << std::endl;
    }
}

///////////////////////////////////////////////////////////////////////////////



void Board::begin()
{
    if(m_autoMoveTimer -> isActive())
    {
        return;
    }
    m_autoMoveTimer -> start(m_timerSpeed); // timer in ms
}









void Board::keyPressEvent(QKeyEvent *event)
{
    if(m_endOfGame == FALSE)
    {
        switch (event -> key())
        {
            case Qt::Key_Up:
                if(m_previousDirection != 1 && m_previousDirection != 3)
                {
                    m_direction = 1;
                    m_repositionOnGrid = TRUE;
                }
                break;

            case Qt::Key_Down:
                if(m_previousDirection != 3 && m_previousDirection != 1)
                {
                    m_direction = 3;
                    m_repositionOnGrid = TRUE;
                }
                break;

            case Qt::Key_Left:
                if(m_previousDirection != 2 && m_previousDirection != 0)
                {
                    m_direction = 2;
                    m_repositionOnGrid = TRUE;
                }
                break;

            case Qt::Key_Right:
                if(m_previousDirection != 0 && m_previousDirection != 2)
                {
                    m_direction = 0;
                    m_repositionOnGrid = TRUE;
                }
                break;

            case Qt::Key_2:
                augmenterLongueur();
                m_score = 0;
                break;

            case Qt::Key_1:
                nouvellePositionPomme();
                m_score = 0;
                break;

            case Qt::Key_3:
                m_score = 0;
                m_steps = 1000;
                m_positionsSauvegardeesPommeX.resize(101);
                gagner();
                break;
        }
    }

    if (!m_autoMoveTimer -> isActive() && m_endOfGame == FALSE)
    {
        begin();
    }

    switch (event -> key())
    {
        case Qt::Key_D:
            m_changementPlateau = 1;
            if(m_quelPlateau != "densite")
            {
                m_quelPlateau = "densite";
            }
            else
            {
                m_quelPlateau = "";
            }
            //AFFICHAGE
            //std::cout << "D" << std::endl;
            break;

        case Qt::Key_Q:
            m_changementPlateau = 1;
            if(m_quelPlateau != "arete")
            {
                m_quelPlateau = "arete";
            }
            else
            {
                m_quelPlateau = "";
            }
            //AFFICHAGE
            //std::cout << "F" << std::endl;
            break;

        case Qt::Key_S:
            m_changementPlateau = 1;
            if(m_quelPlateau != "nonoriente")
            {
                m_quelPlateau = "nonoriente";
            }
            else
            {
                m_quelPlateau = "";
            }
            //AFFICHAGE
            //std::cout << "G" << std::endl;
            break;

        case Qt::Key_P:
            m_changementPlateau = 1;
            if(m_quelPlateau != "pomme")
            {
                m_quelPlateau = "pomme";
            }
            else
            {
                m_quelPlateau = "";
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



void Board::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);

    if(m_changementPlateau == 1)
    {
        paintTout(painter);
    }

    if (m_autoMoveTimer -> isActive())
    {
        if(m_quelPlateau == "")
        {
            paintShot(painter);
        }
        else if(m_quelPlateau == "densite")
        {
            paintDensite(painter);
        }
        else if(m_quelPlateau == "arete")
        {
            paintArete(painter);
        }
        else if(m_quelPlateau == "nonoriente")
        {
            paintNonOriente(painter);
        }
        else if(m_quelPlateau == "pomme")
        {
            paintPomme(painter);
        }
    }

}









void Board::agrandissementSnake()
{
    if(m_nbPasAvantAgrandissementSnake.size() != 0)
    {
        //std::cout << "... ";
        for(unsigned int i(0); i < m_nbPasAvantAgrandissementSnake.size(); i++)
        {
            //std::cout << m_nbPasAvantAgrandissementSnake[i] << " ";
            if(m_nbPasAvantAgrandissementSnake[i] != 0)
            {
                m_nbPasAvantAgrandissementSnake[i]--;
            }
            else
            {
                std::cout << "ERREUR Board::agrandissementSnake() deux augmentations de longueur en même temps." << std::endl;
            }
        }
        //std::cout << std::endl;

        if(m_nbPasAvantAgrandissementSnake[0] == 0) //cf c'est forcément lui qui sonne le premier
        {
            m_nbPasAvantAgrandissementSnake.pop_front();
            m_length += 1;
            m_nePasBougerLaQueue += m_cellSize;
        }
    }
}

void Board::decroitPointEnCours()
{
    //std::cout << m_nbStepsBeforeDecreasingPoints << std::endl;
    if(m_pointsCurrentApple != 1 && m_nbStepsBeforeDecreasingPoints == 0)
    {
        m_pointsCurrentApple = 0.95*m_pointsCurrentApple;
    }
    //std::cout << m_pointsCurrentApple << " sont les points."<< std::endl;
}

void Board::motion()
{
    if(m_nePasBougerLaQueue != 0)
    {
        m_nePasBougerLaQueue--;
    }

    if(m_currentXPosition % m_cellSize == 0 && m_currentYPosition % m_cellSize == 0) //on est sur la grille exactement
    {
        m_steps++;
        //std::cout << m_steps << " c'est le temps " << std::endl;
        if(m_nbStepsBeforeDecreasingPoints != 0)
        {
            m_nbStepsBeforeDecreasingPoints--;
        }
        agrandissementSnake();
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
        if(m_score == 3000)
        {
            gagner();
        }
    }

    if(m_currentXPosition % m_cellSize == 1 || m_currentYPosition % m_cellSize == 1 || m_currentXPosition % m_cellSize == m_cellSize - 1 || m_currentYPosition % m_cellSize == m_cellSize - 1) //on vient de quitter la grille
    {
        testCollision();
    }

    //lié à ancienneDirection (pour éviter des mouvements du snake)
    if( m_repositionOnGrid == FALSE || ( m_currentXPosition % m_cellSize == 0 && m_currentYPosition % m_cellSize == 0) )
    {
        m_previousDirection = m_direction;
        m_repositionOnGrid = FALSE;
    }

    //on avance d'un pas (modulo le plateau)
    motionOneStep();
}

void Board::testCollision()
{
    //test la collision avec les valeurs précédentes
    for(unsigned int i(0); i < std::min(m_length,m_positionsSauvegardeesXmod.size() - 1); i++)
    {
        unsigned int positioniPasAvant(m_positionsSauvegardeesXmod.size() - 2 - i);
        if(m_positionsSauvegardeesXmod.back() == m_positionsSauvegardeesXmod[positioniPasAvant] && m_positionsSauvegardeesYmod.back() == m_positionsSauvegardeesYmod[positioniPasAvant])
        {
            endOfGame();
        }
    }

    //dans les cas où m_length est impaire (donc le snake totale est de taille 4, 6, 8 etc.), test de collision réelle (qui peut survenir ou ne pas survenir suivant les configurations)
    if(m_positionsSauvegardeesXmod.size() >= m_length+2)
    {
        unsigned int positionQueue(m_positionsSauvegardeesXmod.size() - m_length - 2);
        if(m_positionsSauvegardeesXmod.back() == m_positionsSauvegardeesXmod[positionQueue] && m_positionsSauvegardeesYmod.back() == m_positionsSauvegardeesYmod[positionQueue]) //s'il y a une possibilité de collision (cas ambigü)
        {
            if(m_rectHead().intersects(m_rectQueue(0)) == 1) //on fait le test avec les rectangles
            {
                endOfGame();
            }
        }
    }
}

bool Board::collisionAvecPomme()
{
    if(m_positionsSauvegardeesPommeX.back() == m_positionsSauvegardeesXmod.back() && m_positionsSauvegardeesPommeY.back() == m_positionsSauvegardeesYmod.back())
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
    m_nbStepsBeforeDecreasingPoints = mesureDistancePommeTete();

    //AFFICHAGE
    //std::cout << "Nb de pommes mangées : " << m_positionsSauvegardeesPommeX.size() - 1 << std::endl;

    if(m_score + m_pointsCurrentApple < 3000)
    {
        m_score += m_pointsCurrentApple;
        m_pointsCurrentApple = 100;
        //AFFICHAGE
        //std::cout << "	Le score est :" << m_score << "!" << std::endl;
        scoreChanged(m_score);
    }
    else
    {
        m_score = 3000;
    }
}

void Board::sauvegardePositions()
{
    m_directions.push_back(Helpers::convertirDirection(m_direction).c_str());
    if(m_direction == 0)
    {
        m_positionsSauvegardeesX.push_back(m_positionsSauvegardeesX.back()+1);
        m_positionsSauvegardeesY.push_back(m_positionsSauvegardeesY.back());
    }
    if(m_direction == 1)
    {
        m_positionsSauvegardeesX.push_back(m_positionsSauvegardeesX.back());
        m_positionsSauvegardeesY.push_back(m_positionsSauvegardeesY.back() - 1);
    }
    if(m_direction == 2)
    {
        m_positionsSauvegardeesX.push_back(m_positionsSauvegardeesX.back() - 1);
        m_positionsSauvegardeesY.push_back(m_positionsSauvegardeesY.back());
    }
    if(m_direction == 3)
    {
        m_positionsSauvegardeesX.push_back(m_positionsSauvegardeesX.back());
        m_positionsSauvegardeesY.push_back(m_positionsSauvegardeesY.back()+1);
    }
}



void Board::sauvegardePositionsModulo()
{
    m_positionsSauvegardeesXmod.push_back(m_positionsSauvegardeesX.back() % m_nbSquaresX);
    m_positionsSauvegardeesYmod.push_back(m_positionsSauvegardeesY.back() % m_nbSquaresY);
    if(m_positionsSauvegardeesXmod.back() < 0)
    {
        m_positionsSauvegardeesXmod.back() = m_positionsSauvegardeesXmod.back()+m_nbSquaresX;
    }
    if(m_positionsSauvegardeesYmod.back() < 0)
    {
        m_positionsSauvegardeesYmod.back() = m_positionsSauvegardeesYmod.back()+m_nbSquaresY;
    }
}

void Board::sauvegardeArete()
{
    int xAvant(m_positionsSauvegardeesXmod.back());
    int yAvant(m_positionsSauvegardeesYmod.back());

    //int xAvantAvant(m_positionsSauvegardeesXmod[m_positionsSauvegardeesXmod.size() - 2]);
    //int yAvantAvant(m_positionsSauvegardeesYmod[m_positionsSauvegardeesYmod.size() - 2]);
    int aTesterValeur(-1);

    if(m_direction == 0)
    {
        m_aretesEst[m_nbSquaresX*yAvant+xAvant] += 1;
        aTesterValeur = m_aretesEst[m_nbSquaresX*yAvant+xAvant];
        //std::cout << xAvant << " " << yAvant << " " << "est" << " " << m_aretesEst[m_nbSquaresX*yAvant+xAvant] << std::endl;
    }
    else if(m_direction == 1)
    {
        m_aretesNord[m_nbSquaresX*yAvant+xAvant] += 1;
        aTesterValeur = m_aretesNord[m_nbSquaresX*yAvant+xAvant];
        //std::cout << xAvant << " " << yAvant << " " << "nord" << " " << m_aretesNord[m_nbSquaresX*yAvant+xAvant] << std::endl;
    }
    else if(m_direction == 2)
    {
        m_aretesOuest[m_nbSquaresX*yAvant+xAvant] += 1;
        aTesterValeur = m_aretesOuest[m_nbSquaresX*yAvant+xAvant];
        //std::cout << xAvant << " " << yAvant << " " << "ouest" << " " << m_aretesOuest[m_nbSquaresX*yAvant+xAvant] << std::endl;
    }
    else if(m_direction == 3)
    {
        m_aretesSud[m_nbSquaresX*yAvant+xAvant] += 1;
        aTesterValeur = m_aretesSud[m_nbSquaresX*yAvant+xAvant];
        //std::cout << xAvant << " " << yAvant << " " << "sud" << " " << m_aretesSud[m_nbSquaresX*yAvant+xAvant] << std::endl;
    }

    //std::cout << m_valeurMaxAreteSauvegardee << std::endl;
    if(m_valeurMaxAreteSauvegardee < aTesterValeur)
    {
        m_valeurMaxAreteSauvegardee = aTesterValeur;
        if(m_quelPlateau == "arete" || m_quelPlateau == "nonoriente")
        {
            m_changementPlateau = 1;
        }
    }

    //std::cout << m_valeurMaxAreteSauvegardee << std::endl;
}

void Board::sauvegardeGrille()
{
    int xAvant(m_positionsSauvegardeesXmod.back());
    int yAvant(m_positionsSauvegardeesYmod.back());
    m_grilleSauvegardee[m_nbSquaresX*yAvant+xAvant] += 1;
    if(m_valeurMaxGrilleSauvegardee < m_grilleSauvegardee[m_nbSquaresX*yAvant+xAvant])
    {
        m_valeurMaxGrilleSauvegardee = m_grilleSauvegardee[m_nbSquaresX*yAvant+xAvant];
        if(m_quelPlateau == "densite")
        {
            m_changementPlateau = 1;
        }
    }
    //std::cout << xAvant << " " << yAvant << " " << m_grilleSauvegardee[m_nbSquaresX*yAvant+xAvant] << std::endl;
}

void Board::motionOneStep()
{
    if(m_previousDirection == 0)
    {
        m_currentXPosition++;
    }
    if(m_previousDirection == 1)
    {
        m_currentYPosition--;
    }
    if(m_previousDirection == 2)
    {
        m_currentXPosition--;
    }
    if(m_previousDirection == 3)
    {
        m_currentYPosition++;
    }

    if(m_currentYPosition < 0)
    {
        m_currentYPosition += m_cellSize*m_nbSquaresY;
    }
    if(m_currentYPosition >= m_cellSize*m_nbSquaresY)
    {
        m_currentYPosition -= m_cellSize*m_nbSquaresY;
    }
    if(m_currentXPosition < 0)
    {
        m_currentXPosition += m_cellSize*m_nbSquaresX;
    }
    if(m_currentXPosition >= m_cellSize*m_nbSquaresX)
    {
        m_currentXPosition -= m_cellSize*m_nbSquaresX;
    }
}


void Board::writingHiScore()
{
    if(m_hiScore < m_score)
    {
        //AFFICHAGE
        //std::cout << m_score << " score final" << std::endl;
        QFile data2("hiscore.txt");
        if(data2.open(QFile::WriteOnly))
        {
            QTextStream out2(&data2);
            out2 << Helpers::base(m_score,9) << "	" << Helpers::base(m_score,7) << "	" << Helpers::base(m_score,8);
        }
        /*
          std::string test = "45";
          int myint = std::stoi(test);
          std::cout << myint << '\n';
        */
        //std::cout << Helpers::base(m_score,2) << std::endl;
    }
}







void Board::augmenterLongueur()
{
    m_nbPasAvantAgrandissementSnake.push_back(m_length+1);
}

void Board::endOfGame()
{
    m_endOfGame = TRUE;
    //AFFICHAGE
    //std::cout << m_score << " score final" << std::endl;
    writingWholeHistory();
    writingHiScore();
}

void Board::gagner()
{
    //AFFICHAGE
    //std::cout << "GAGNE !" << std::endl;
    m_win = TRUE;
    m_changementPlateau = 1;
    if(m_steps <= 1000)
    {
        m_score += 10*(1000 - m_steps);
    }

    if(m_positionsSauvegardeesPommeX.size() - 1 <= 100)
    {
        m_score += 100*(100 - (m_positionsSauvegardeesPommeX.size() - 1));
    }

    scoreChanged(m_score);
    endOfGame();
}

void Board::nouvellePositionPomme()
{
    //int X(qrand() % m_nbSquaresX);
    //int Y(qrand() % m_nbSquaresY);
    int X(-1);
    int Y(-1);
    while(X == -1)
    {
        int nouveauX(Alea::rand_integer(m_nbSquaresX));
        int nouveauY(Alea::rand_integer(m_nbSquaresY));
        //std::cout << nouveauX << " " << nouveauY << std::endl;
        if(pasDeCollisionAvecSerpent(nouveauX,nouveauY) || m_endOfGame == TRUE)
        {
            X = nouveauX;
            Y = nouveauY;
        }
    }

    if(m_endOfGame == FALSE)
    {
        m_positionPommeX = m_cellSize*X;
        m_positionPommeY = m_cellSize*Y;
        m_positionsSauvegardeesPommeX.push_back(X);
        m_positionsSauvegardeesPommeY.push_back(Y);
        //std::cout << "Pomme : " <<  m_positionPommeX << " " << m_positionPommeY << std::endl;

        sauvegardePomme();
    }
}

bool Board::pasDeCollisionAvecSerpent(int nouveauX, int nouveauY)
{
    if((int) m_length + 1 == m_nbSquaresX*m_nbSquaresY)
    {
        sauvegardePositions(); //enregistre position en cours dans m_positionsSauvegardeesX/Y
        sauvegardePositionsModulo(); //idem modulo la taille du plateau (méthode à lancer après sauvegardePositions())
        gagner();
        return 1;
    }

    for(unsigned int i(0); i < m_length+1; i++)
    {
        //std::cout << "test avec : " << m_positionsSauvegardeesXmod[m_positionsSauvegardeesXmod.size() - 1 - i] << " " << m_positionsSauvegardeesYmod[m_positionsSauvegardeesYmod.size() - 1 - i] << std::endl;
        if(m_positionsSauvegardeesXmod[m_positionsSauvegardeesXmod.size() - 1 - i] == nouveauX && m_positionsSauvegardeesYmod[m_positionsSauvegardeesYmod.size() - 1 - i] == nouveauY)
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

    m_grillePomme[m_nbSquaresX*yAvant+xAvant] += 1;

    if(m_valeurMaxGrillePomme < m_grillePomme[m_nbSquaresX*yAvant+xAvant])
    {
        m_valeurMaxGrillePomme = m_grillePomme[m_nbSquaresX*yAvant+xAvant];
        if(m_quelPlateau == "pomme")
        {
            m_changementPlateau = 1;
        }
    }
    //std::cout << "pommy " << xAvant << " " << yAvant << " " <<  m_grillePomme[m_nbSquaresX*yAvant+xAvant] << std::endl;
}

int Board::mesureDistancePommeTete()
{
    int appleX(m_positionPommeX/m_cellSize);
    int appleY(m_positionPommeY/m_cellSize);

    int headX(m_currentXPosition/m_cellSize);
    int headY(m_currentYPosition/m_cellSize);

    int distX(std::min(abs(headX - appleX), m_nbSquaresX - abs(headX - appleX)));
    int distY(std::min(abs(headY - appleY), m_nbSquaresY - abs(headY - appleY)));

    // DEBUG
    // std::cout << distX << " " << distY << std::endl;

    return distX + distY;
}



























































void Board::paintShot(QPainter &painter)
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    if(m_win == TRUE)
    {
        painter.setBrush(QColor(60, 60, 200));
    }

    //dessiner corps
    for(unsigned int i(0); i < std::min(m_positionsSauvegardeesXmod.size() - 1, m_length); i++)
    {
        painter.drawRect(m_rectCorps(m_positionsSauvegardeesXmod.size() - i - 2,0));
    }


    //dessiner queue mouvante
    if(m_positionsSauvegardeesXmod.size() > m_length + 1)
    {
        //painter.setBrush(Qt::blue);
        painter.drawRect(m_rectQueue(0));
    }


    // Head
    //painter.setBrush(Qt::green);


    painter.drawRect(m_rectHeadCorner(0));

    if(m_endOfGame == TRUE)
    {
        painter.setBrush(Qt::red);
    }
    if(m_win == TRUE)
    {
        painter.setBrush(QColor(80, 80, 200));
    }

    painter.drawRect(m_rectHeadUsual(0));



    // Apple

    QRect pomme(m_rectPomme(0)); //placé au début, on le déplace dans la ligne suivante
    painter.setPen(Qt::NoPen);

    //painter.setBrush(Qt::green);
    painter.setBrush(QColor(std::min(2*(100 - m_pointsCurrentApple),128),
                            std::max(255 - 1*(100 - m_pointsCurrentApple),128),
                            std::min(2*(100 - m_pointsCurrentApple),128)));

    if(m_win == FALSE)
    {
        painter.drawRect(pomme);
    }



}




















/*
  Painting the whole board
*/
void Board::paintTout(QPainter &painter)
{
    //setPalette(QPalette(QColor(250, 250, 200)));
    painter.setPen(Qt::NoPen);
    if(m_quelPlateau == "densite" || m_quelPlateau == "arete" || m_quelPlateau == "nonoriente" || m_quelPlateau == "pomme")
    {
        painter.setBrush(Qt::white);
    }
    else
    {
        painter.setBrush(QColor(250, 250, 200));
    }

    painter.drawRect(m_rectPlateau());
}

/*
  Current layer and painting statistic board layers
*/
// Snake's density layer
void Board::paintDensite(QPainter &painter)
{
    painter.setPen(Qt::NoPen);

    int minTransp(20);
    int maxTransp(240);
    int pas((maxTransp - minTransp)/(m_valeurMaxGrilleSauvegardee + 1));

    for(int y(0); y < m_nbSquaresY; y++)
    {
        for(int x(0); x < m_nbSquaresX; x++)
        {
            painter.setBrush(QColor(0, 0, 255, minTransp + pas*m_grilleSauvegardee[y*m_nbSquaresX + x]));
            painter.drawRect(m_rect(x* m_cellSize, y* m_cellSize));
        }
       }
}

// Oriented edges crossed
void Board::paintArete(QPainter &painter)
{
    painter.setPen(Qt::NoPen);

    int minTransp(20);
    int maxTransp(240);
    int pas((maxTransp - minTransp)/(m_valeurMaxAreteSauvegardee + 1));

    for(int y(0); y < m_nbSquaresY; y++)
    {
        for(int x(0); x < m_nbSquaresX; x++)
        {
            painter.setBrush(QColor(255, 0, 0, minTransp + pas*m_aretesEst[y*m_nbSquaresX + x]));
            painter.drawPolygon(m_triangleEst(x,y));

            painter.setBrush(QColor(255, 0, 0, minTransp + pas*m_aretesNord[y*m_nbSquaresX + x]));
            painter.drawPolygon(m_triangleNord(x,y));

            painter.setBrush(QColor(255, 0, 0, minTransp + pas*m_aretesOuest[y*m_nbSquaresX + x]));
            painter.drawPolygon(m_triangleOuest(x,y));

            painter.setBrush(QColor(255, 0, 0, minTransp + pas*m_aretesSud[y*m_nbSquaresX + x]));
            painter.drawPolygon(m_triangleSud(x,y));
        }
    }
}



// Non-oriented edges crossed
void Board::paintNonOriente(QPainter &painter)
{
    painter.setPen(Qt::NoPen);

    int minTransp(20);
    int maxTransp(240);
    int pas((maxTransp - minTransp)/(2*m_valeurMaxAreteSauvegardee));

    for(int y(0); y < m_nbSquaresY; y++)
    {
        for(int x(0); x < m_nbSquaresX; x++)
        {
            if(x != 0)
            {
                painter.setBrush(QColor(255, 0, 0, minTransp + pas*(m_aretesEst[y*m_nbSquaresX + x] + m_aretesOuest[y*m_nbSquaresX + (x - 1)])));
                painter.drawPolygon(m_triangleEst(x,y));

                painter.setBrush(QColor(255, 0, 0, minTransp + pas*(m_aretesEst[y*m_nbSquaresX + x] + m_aretesOuest[y*m_nbSquaresX + (x - 1)])));
                painter.drawPolygon(m_triangleOuest(x - 1,y));
            }
            else
            {
                painter.setBrush(QColor(255, 0, 0, minTransp + pas*(m_aretesEst[y*m_nbSquaresX + x] + m_aretesOuest[y*m_nbSquaresX + (x - 1) + m_nbSquaresX])));
                painter.drawPolygon(m_triangleEst(x,y));

                painter.setBrush(QColor(255, 0, 0, minTransp + pas*(m_aretesEst[y*m_nbSquaresX + x] + m_aretesOuest[y*m_nbSquaresX + (x - 1) + m_nbSquaresX])));
                painter.drawPolygon(m_triangleOuest(x - 1 + m_nbSquaresX,y));
            }

            if(y != 0)
            {
                painter.setBrush(QColor(255, 0, 0, minTransp + pas*(m_aretesNord[(y - 1)*m_nbSquaresX + x] + m_aretesSud[y*m_nbSquaresX + x])));
                painter.drawPolygon(m_triangleNord(x,y - 1));

                painter.setBrush(QColor(255, 0, 0, minTransp + pas*(m_aretesNord[(y - 1)*m_nbSquaresX + x] + m_aretesSud[y*m_nbSquaresX + x])));
                painter.drawPolygon(m_triangleSud(x,y));
            }
            else
            {
                painter.setBrush(QColor(255, 0, 0, minTransp + pas*(m_aretesNord[(y - 1 + m_nbSquaresY)*m_nbSquaresX + x] + m_aretesSud[y*m_nbSquaresX + x])));
                painter.drawPolygon(m_triangleNord(x,y - 1 + m_nbSquaresY));

                painter.setBrush(QColor(255, 0, 0, minTransp + pas*(m_aretesNord[(y - 1 + m_nbSquaresY)*m_nbSquaresX + x] + m_aretesSud[y*m_nbSquaresX + x])));
                painter.drawPolygon(m_triangleSud(x,y));
            }
        }
    }
}

// Apple's density layer
void Board::paintPomme(QPainter &painter)
{
    painter.setPen(Qt::NoPen);
    int minTransp(20);
    int maxTransp(240);
    int pas((maxTransp - minTransp)/(m_valeurMaxGrillePomme + 1));
    //std::cout << m_valeurMaxGrillePomme << " est nb pommes" << std::endl;
    for(int y(0); y < m_nbSquaresY; y++)
    {
        for(int x(0); x < m_nbSquaresX; x++)
        {
            //std::cout << x << "	" << y << "	" << m_grillePomme[y*m_nbSquaresX + x] << std::endl;
            painter.setBrush(QColor(0, 255, 0, minTransp + pas*m_grillePomme[y*m_nbSquaresX + x]));
            painter.drawRect(m_rect(x* m_cellSize, y* m_cellSize));
        }
    }
}

////////////////////////
// Whole game history //
////////////////////////
void Board::sauvegardeTempsPointsNbPommesMangeesLongueurSnake()
{
    m_stepsSave.push_back(m_steps);
    m_scoreSave.push_back(m_score);
    m_nbPommesMangeesSave.push_back(m_positionsSauvegardeesPommeX.size() - 1);
    m_lengthSnakeSave.push_back(m_length + 1);
    m_positionsSauvegardeesPommeXcomplet.push_back(m_positionsSauvegardeesPommeX.back());
    m_positionsSauvegardeesPommeYcomplet.push_back(m_positionsSauvegardeesPommeY.back());
}

void Board::writingWholeHistory()
{
    QFile data("output.txt");
    if(data.open(QFile::WriteOnly | QIODevice::Append))
    {
        QTextStream out(&data);
        //out << "9999 9999" << endl; //endl de la classe QTextStream, pas de std.

        //std::cout << m_stepsSave.size() << " " << m_scoreSave.size() << " " << m_nbPommesMangeesSave.size() << " " << m_lengthSnakeSave.size() << " " <<  m_positionsSauvegardeesX.size() << " " << m_positionsSauvegardeesY.size() << " " << m_positionsSauvegardeesPommeXcomplet.size() << " " << m_positionsSauvegardeesPommeYcomplet.size() << std::endl;

        if(m_currentPlayId == 0)
        {
            out << "partieEnCours" << "	" << "temps" << "	" << "score" << "	"
                << "nbPommesMangees" << "	" << "longueurSnake" << "	"
                <<  "positionsX" << "	" << "positionsY" << "	"
                 << "positionsPommeX" << "	" << "positionsPommeY"
                 << endl;
        }

        for(unsigned int i(0); i < m_positionsSauvegardeesX.size(); i++)
        {
            out << m_currentPlayId << "	" << m_stepsSave[i] << "	" << m_scoreSave[i] << "	"
                << m_nbPommesMangeesSave[i] << "	" << m_lengthSnakeSave[i] << "	"
                <<  m_positionsSauvegardeesX[i] << "	" << m_positionsSauvegardeesY[i] << "	"
                << m_positionsSauvegardeesPommeXcomplet[i] << "	" << m_positionsSauvegardeesPommeYcomplet[i]
                << endl;
        }
    }
}
