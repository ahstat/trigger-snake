#include "Board.h"

/*****************
* Public methods *
*****************/
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

/*******
* Slot *
*******/
// There is only one slot, which continuously waits for user actions and
// board updates. It is called by newgame().
void Board::updateMove()
{
    // Update the whole board
    update(m_board());
    // Note: Doing too many updates will slow down the game

    if(m_endOfGame == FALSE)
    {
        // Motion of the snake
        motion();
    }
}

/******************
* Private methods *
******************/
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
    m_nbStepsBeforeDecreasingPoints = distanceAppleSnake();


    ////
    // Saving all states of the game
    ////
    // todo

    //informations sauvegardées du snake (pour les datas)
    m_savedXPositions.resize(0);
    m_savedYPositions.resize(0);
    m_savedXPositions.push_back(m_currentXPosition / m_cellSize);
    m_savedYPositions.push_back(m_currentYPosition/m_cellSize); //cf initialement, position en (0,0) sur la grille
    m_savedXPositionsMod.resize(0);
    m_savedYPositionsMod.resize(0);
    m_savedXPositionsMod.push_back(m_currentXPosition/m_cellSize);
    m_savedYPositionsMod.push_back(m_currentYPosition/m_cellSize); //cf initialement, position en (0,0) sur la grille
    m_savedDirections.resize(0);
    m_savedDensity.resize(m_nbSquaresX*m_nbSquaresY);
    m_savedApple.resize(m_nbSquaresX*m_nbSquaresY);
    m_savedEastEdges.resize(m_nbSquaresX*m_nbSquaresY);
    m_savedNorthEdges.resize(m_nbSquaresX*m_nbSquaresY);
    m_savedWestEdges.resize(m_nbSquaresX*m_nbSquaresY);
    m_savedSouthEdges.resize(m_nbSquaresX*m_nbSquaresY);

    for(unsigned int i(0); i < m_savedDensity.size(); i++)
    {
        m_savedDensity[i] = 0;
        m_savedApple[i] = 0;
        m_savedEastEdges[i] = 0;
        m_savedNorthEdges[i] = 0;
        m_savedWestEdges[i] = 0;
        m_savedSouthEdges[i] = 0;
    }

    setPalette(QPalette(QColor(250, 250, 200)));
    setAutoFillBackground(true);
    setFixedSize(m_cellSize * m_nbSquaresX, m_cellSize * m_nbSquaresY);

    saveDensity();

    m_savedXPositionsApple.resize(0);
    m_savedYPositionsApple.resize(0);


    m_nbStepsBeforeIncreasingSnake.resize(0);
    m_nbPixDoNotMoveTail = 0;

    newApplePosition();

    m_isBoardLayerChanged = 0;
    m_currentBoard = "";
    m_savedMaxDensityValue = 0;
    m_savedMaxEdgeValue = 0;

    m_savedMaxAppleValue = 0;

    m_savedAllSteps.resize(0);
    m_savedAllScore.resize(0);
    m_savedAllNbApples.resize(0);
    m_savedAllSnakeLength.resize(0);
    m_savedAllSteps.push_back(0);
    m_savedAllScore.push_back(0);
    m_savedAllNbApples.push_back(0);
    m_savedAllSnakeLength.push_back(m_length+1);

    m_savedAllXPositionsApple.resize(0);
    m_savedAllYPositionsApple.resize(0);
    m_savedAllXPositionsApple.push_back(m_savedXPositionsApple.back());
    m_savedAllYPositionsApple.push_back(m_savedYPositionsApple.back());

    m_isBoardLayerChanged = 1;
    updateMove();

    m_autoMoveTimer -> stop();
}

QRect Board::m_board()
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

QPolygon Board::m_northTriangle(int x, int y)
{
    QPolygon triangleN(3);
    triangleN.setPoint(0, x*m_cellSize, y*m_cellSize+m_cellSize);
    triangleN.setPoint(1, x*m_cellSize+m_cellSize, y*m_cellSize+m_cellSize);
    triangleN.setPoint(2, x*m_cellSize+m_cellSize/2, y*m_cellSize+m_cellSize/2);
    return triangleN;
}

QPolygon Board::m_southTriangle(int x, int y)
{
    QPolygon triangleN(3);
    triangleN.setPoint(0, x*m_cellSize, y*m_cellSize);
    triangleN.setPoint(1, x*m_cellSize+m_cellSize, y*m_cellSize);
    triangleN.setPoint(2, x*m_cellSize+m_cellSize/2, y*m_cellSize+m_cellSize/2);
    return triangleN;
}

QPolygon Board::m_eastTriangle(int x, int y)
{
    QPolygon triangleN(3);
    triangleN.setPoint(0, x*m_cellSize, y*m_cellSize);
    triangleN.setPoint(1, x*m_cellSize, y*m_cellSize+m_cellSize);
    triangleN.setPoint(2, x*m_cellSize+m_cellSize/2, y*m_cellSize+m_cellSize/2);
    return triangleN;
}

QPolygon Board::m_westTriangle(int x, int y)
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

QRect Board::m_rectHeadUsual(bool isToBeDiplayed)
{
    QRect result(m_rect(m_currentXPosition, m_currentYPosition));
    if(m_endOfGame == TRUE)
    {
        // Position of snake's head is took from saved positions
        int indexBeforeBump = m_savedXPositionsMod.size() - 2;
        int cellXBeforeBump = m_savedXPositionsMod[indexBeforeBump];
        int cellYBeforeBump = m_savedYPositionsMod[indexBeforeBump];
        // Move to correct position in pixels
        result.moveTopLeft(QPoint(cellXBeforeBump * m_cellSize, cellYBeforeBump * m_cellSize));
    }

    display(isToBeDiplayed, "m_rectHeadUsual", result);
    return result;
}

QRect Board::m_rectHeadCorner(bool isToBeDiplayed)
{
    if(m_currentYPosition > m_cellSize*m_nbSquaresY - m_cellSize)
    {
        QRect result(m_rect(m_currentXPosition, m_currentYPosition - m_cellSize*m_nbSquaresY));
        display(isToBeDiplayed, "m_rectHeadCorner Y", result);
        return result;
    }
    else if(m_currentXPosition > m_cellSize*m_nbSquaresX - m_cellSize)
    {
        QRect result(m_rect(m_currentXPosition - m_cellSize*m_nbSquaresX, m_currentYPosition));
        display(isToBeDiplayed, "m_rectHeadCorner X", result);
        return result;
    }
    else
    {
        return QRect();
    }
}

QRect Board::m_rectBody(unsigned int i, bool isToBeDiplayed)
{
    int cellX = m_savedXPositionsMod[i];
    int cellY = m_savedYPositionsMod[i];
    QRect result(m_rect(cellX*m_cellSize, cellY*m_cellSize));

    display(isToBeDiplayed, "m_rectBody", result);
    return result;
}

QRect Board::m_rectTail(bool isToBeDiplayed)
{
    unsigned int i(m_savedXPositions.size() - m_length - 2);
    QRect result = QRect();

    std::cout << m_currentXPosition << std::endl;

    if(m_endOfGame == TRUE)
    {
        int cellX(m_savedXPositionsMod[i]);
        int cellY(m_savedYPositionsMod[i]);
        result = m_rect(cellX*m_cellSize, cellY*m_cellSize);
    }
    else if(m_nbPixDoNotMoveTail != 0)
    {
        int cellX(m_savedXPositionsMod[i+1]);
        int cellY(m_savedYPositionsMod[i+1]);
        result = m_rect(cellX*m_cellSize, cellY*m_cellSize);
    }
    else
    {
        int cellX(m_savedXPositionsMod[i]);
        int cellY(m_savedYPositionsMod[i]);

        int ajout(0);
        if(m_savedDirections.back() == "est")
        {
            ajout = (m_currentXPosition + m_cellSize - 1) % m_cellSize;// + 1;
        }
        else if(m_savedDirections.back() == "ouest")
        {
            ajout = m_cellSize - m_currentXPosition % m_cellSize;
        }
        else if(m_savedDirections.back() == "nord")
        {
            ajout = m_cellSize - m_currentYPosition % m_cellSize;
        }
        else if(m_savedDirections.back() == "sud")
        {
            ajout = (m_currentYPosition+m_cellSize - 1) % m_cellSize;// + 1;
        }

        std::string direction(m_savedDirections[m_savedXPositions.size() - m_length - 2]);
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
    }

    display(isToBeDiplayed, "m_rectTail", result);
    return result;
}

QRect Board::m_rectApple(bool isToBeDiplayed)
{
    QRect result(m_rect(m_currentXApple, m_currentYApple));

    display(isToBeDiplayed, "m_rectApple", result);
    return result;
}

void Board::display(bool isToBeDiplayed, std::string objName, QRect result) const
{
    if(isToBeDiplayed == TRUE)
    {
        std::cout << objName;
        std::cout << " (" << result.x() << "," << result.y() << ")" << std::endl;
    }
}

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
                planAnIncreaseOfTheSnake();
                m_score = 0;
                break;

            case Qt::Key_1:
                newApplePosition();
                m_score = 0;
                break;

            case Qt::Key_3:
                m_score = 0;
                m_steps = 1000;
                m_savedXPositionsApple.resize(101);
                win();
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
            m_isBoardLayerChanged = 1;
            if(m_currentBoard != "density")
            {
                m_currentBoard = "density";
            }
            else
            {
                m_currentBoard = "";
            }
            break;

        case Qt::Key_Q:
            m_isBoardLayerChanged = 1;
            if(m_currentBoard != "edge")
            {
                m_currentBoard = "edge";
            }
            else
            {
                m_currentBoard = "";
            }
            break;

        case Qt::Key_S:
            m_isBoardLayerChanged = 1;
            if(m_currentBoard != "nonoriented")
            {
                m_currentBoard = "nonoriented";
            }
            else
            {
                m_currentBoard = "";
            }
            break;

        case Qt::Key_P:
            m_isBoardLayerChanged = 1;
            if(m_currentBoard != "apple")
            {
                m_currentBoard = "apple";
            }
            else
            {
                m_currentBoard = "";
            }
            break;


        case Qt::Key_R:
            newGame();
            break;
    }
}

void Board::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);

    if(m_isBoardLayerChanged == 1)
    {
        paintWholeBoard(painter);
    }

    if (m_autoMoveTimer -> isActive())
    {
        if(m_currentBoard == "")
        {
            paintShot(painter);
        }
        else if(m_currentBoard == "density")
        {
            paintDensity(painter);
        }
        else if(m_currentBoard == "edge")
        {
            paintEdge(painter);
        }
        else if(m_currentBoard == "nonoriented")
        {
            paintNonOriented(painter);
        }
        else if(m_currentBoard == "apple")
        {
            paintApple(painter);
        }
    }

}

void Board::increaseSnake()
{
    if(m_nbStepsBeforeIncreasingSnake.size() != 0)
    {
        for(unsigned int i(0); i < m_nbStepsBeforeIncreasingSnake.size(); i++)
        {
            if(m_nbStepsBeforeIncreasingSnake[i] != 0)
            {
                m_nbStepsBeforeIncreasingSnake[i]--;
            }
            else
            {
                std::cout << "ERREUR Board::increaseSnake() deux augmentations de longueur en même temps." << std::endl;
            }
        }

        if(m_nbStepsBeforeIncreasingSnake[0] == 0) //cf c'est forcément lui qui sonne le premier
        {
            m_nbStepsBeforeIncreasingSnake.pop_front();
            m_length += 1;
            m_nbPixDoNotMoveTail += m_cellSize;
        }
    }
}

void Board::decreasingPoints()
{
    if(m_pointsCurrentApple != 1 && m_nbStepsBeforeDecreasingPoints == 0)
    {
        m_pointsCurrentApple = 0.95*m_pointsCurrentApple;
    }
}

void Board::motion()
{
    if(m_nbPixDoNotMoveTail != 0)
    {
        m_nbPixDoNotMoveTail--;
    }

    if(m_currentXPosition % m_cellSize == 0 && m_currentYPosition % m_cellSize == 0) //on est sur la grille exactement
    {
        m_steps++;
        if(m_nbStepsBeforeDecreasingPoints != 0)
        {
            m_nbStepsBeforeDecreasingPoints--;
        }
        increaseSnake();
        savePositions(); //ajout data
        savePositionsMod(); //ajout data modulo le plateau
        saveDensity();
        saveEdge();

        decreasingPoints();

        if(collisionWithApple())
        {
            eatenApple();
        }

        saveAllHistory();
        if(m_score == 3000)
        {
            win();
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
    for(unsigned int i(0); i < std::min(m_length,m_savedXPositionsMod.size() - 1); i++)
    {
        unsigned int positioniPasAvant(m_savedXPositionsMod.size() - 2 - i);
        if(m_savedXPositionsMod.back() == m_savedXPositionsMod[positioniPasAvant] && m_savedYPositionsMod.back() == m_savedYPositionsMod[positioniPasAvant])
        {
            endOfGame();
        }
    }

    //dans les cas où m_length est impaire (donc le snake totale est de taille 4, 6, 8 etc.), test de collision réelle (qui peut survenir ou ne pas survenir suivant les configurations)
    if(m_savedXPositionsMod.size() >= m_length+2)
    {
        unsigned int positionQueue(m_savedXPositionsMod.size() - m_length - 2);
        if(m_savedXPositionsMod.back() == m_savedXPositionsMod[positionQueue] && m_savedYPositionsMod.back() == m_savedYPositionsMod[positionQueue]) //s'il y a une possibilité de collision (cas ambigü)
        {
            if(m_rectHead().intersects(m_rectTail(0)) == 1) //on fait le test avec les rectangles
            {
                endOfGame();
            }
        }
    }
}

bool Board::collisionWithApple()
{
    if(m_savedXPositionsApple.back() == m_savedXPositionsMod.back() && m_savedYPositionsApple.back() == m_savedYPositionsMod.back())
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void Board::eatenApple()
{
    planAnIncreaseOfTheSnake();
    newApplePosition();
    m_nbStepsBeforeDecreasingPoints = distanceAppleSnake();

    if(m_score + m_pointsCurrentApple < 3000)
    {
        m_score += m_pointsCurrentApple;
        m_pointsCurrentApple = 100;
        scoreChanged(m_score);
    }
    else
    {
        m_score = 3000;
    }
}

void Board::savePositions()
{
    m_savedDirections.push_back(Helpers::convertirDirection(m_direction).c_str());
    if(m_direction == 0)
    {
        m_savedXPositions.push_back(m_savedXPositions.back()+1);
        m_savedYPositions.push_back(m_savedYPositions.back());
    }
    if(m_direction == 1)
    {
        m_savedXPositions.push_back(m_savedXPositions.back());
        m_savedYPositions.push_back(m_savedYPositions.back() - 1);
    }
    if(m_direction == 2)
    {
        m_savedXPositions.push_back(m_savedXPositions.back() - 1);
        m_savedYPositions.push_back(m_savedYPositions.back());
    }
    if(m_direction == 3)
    {
        m_savedXPositions.push_back(m_savedXPositions.back());
        m_savedYPositions.push_back(m_savedYPositions.back()+1);
    }
}

void Board::savePositionsMod()
{
    m_savedXPositionsMod.push_back(m_savedXPositions.back() % m_nbSquaresX);
    m_savedYPositionsMod.push_back(m_savedYPositions.back() % m_nbSquaresY);
    if(m_savedXPositionsMod.back() < 0)
    {
        m_savedXPositionsMod.back() = m_savedXPositionsMod.back()+m_nbSquaresX;
    }
    if(m_savedYPositionsMod.back() < 0)
    {
        m_savedYPositionsMod.back() = m_savedYPositionsMod.back()+m_nbSquaresY;
    }
}

void Board::saveEdge()
{
    int xAvant(m_savedXPositionsMod.back());
    int yAvant(m_savedYPositionsMod.back());
    int aTesterValeur(-1);

    if(m_direction == 0)
    {
        m_savedEastEdges[m_nbSquaresX*yAvant+xAvant] += 1;
        aTesterValeur = m_savedEastEdges[m_nbSquaresX*yAvant+xAvant];
    }
    else if(m_direction == 1)
    {
        m_savedNorthEdges[m_nbSquaresX*yAvant+xAvant] += 1;
        aTesterValeur = m_savedNorthEdges[m_nbSquaresX*yAvant+xAvant];
    }
    else if(m_direction == 2)
    {
        m_savedWestEdges[m_nbSquaresX*yAvant+xAvant] += 1;
        aTesterValeur = m_savedWestEdges[m_nbSquaresX*yAvant+xAvant];
    }
    else if(m_direction == 3)
    {
        m_savedSouthEdges[m_nbSquaresX*yAvant+xAvant] += 1;
        aTesterValeur = m_savedSouthEdges[m_nbSquaresX*yAvant+xAvant];
    }

    if(m_savedMaxEdgeValue < aTesterValeur)
    {
        m_savedMaxEdgeValue = aTesterValeur;
        if(m_currentBoard == "edge" || m_currentBoard == "nonoriented")
        {
            m_isBoardLayerChanged = 1;
        }
    }
}

void Board::saveDensity()
{
    int xAvant(m_savedXPositionsMod.back());
    int yAvant(m_savedYPositionsMod.back());
    m_savedDensity[m_nbSquaresX*yAvant+xAvant] += 1;
    if(m_savedMaxDensityValue < m_savedDensity[m_nbSquaresX*yAvant+xAvant])
    {
        m_savedMaxDensityValue = m_savedDensity[m_nbSquaresX*yAvant+xAvant];
        if(m_currentBoard == "density")
        {
            m_isBoardLayerChanged = 1;
        }
    }
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
        QFile data2("hiscore.txt");
        if(data2.open(QFile::WriteOnly))
        {
            QTextStream out2(&data2);
            out2 << Helpers::base(m_score,9) << "	" << Helpers::base(m_score,7) << "	" << Helpers::base(m_score,8);
        }
    }
}

void Board::planAnIncreaseOfTheSnake()
{
    m_nbStepsBeforeIncreasingSnake.push_back(m_length+1);
}

void Board::endOfGame()
{
    m_endOfGame = TRUE;
    writingAllHistory();
    writingHiScore();
}

void Board::win()
{
    m_win = TRUE;
    m_isBoardLayerChanged = 1;
    if(m_steps <= 1000)
    {
        m_score += 10*(1000 - m_steps);
    }

    if(m_savedXPositionsApple.size() - 1 <= 100)
    {
        m_score += 100*(100 - (m_savedXPositionsApple.size() - 1));
    }

    scoreChanged(m_score);
    endOfGame();
}

void Board::newApplePosition()
{
    int X(-1);
    int Y(-1);
    while(X == -1)
    {
        int newX(Alea::rand_integer(m_nbSquaresX));
        int newY(Alea::rand_integer(m_nbSquaresY));
        if(!collisionWithSnake(newX,newY) || m_endOfGame == TRUE)
        {
            X = newX;
            Y = newY;
        }
    }

    if(m_endOfGame == FALSE)
    {
        m_currentXApple = m_cellSize*X;
        m_currentYApple = m_cellSize*Y;
        m_savedXPositionsApple.push_back(X);
        m_savedYPositionsApple.push_back(Y);
        saveApple();
    }
}

bool Board::collisionWithSnake(int newX, int newY)
{
    if((int) m_length + 1 == m_nbSquaresX*m_nbSquaresY)
    {
        savePositions(); //enregistre position en cours dans m_savedXPositions/Y
        savePositionsMod(); //idem modulo la taille du plateau (méthode à lancer après savePositions())
        win();
        return 0;
    }

    for(unsigned int i(0); i < m_length+1; i++)
    {
        if(m_savedXPositionsMod[m_savedXPositionsMod.size() - 1 - i] == newX && m_savedYPositionsMod[m_savedYPositionsMod.size() - 1 - i] == newY)
        {
            return 1;
        }
    }
    return 0;
}

void Board::saveApple()
{
    int xAvant(m_savedXPositionsApple.back());
    int yAvant(m_savedYPositionsApple.back());

    m_savedApple[m_nbSquaresX*yAvant+xAvant] += 1;

    if(m_savedMaxAppleValue < m_savedApple[m_nbSquaresX*yAvant+xAvant])
    {
        m_savedMaxAppleValue = m_savedApple[m_nbSquaresX*yAvant+xAvant];
        if(m_currentBoard == "apple")
        {
            m_isBoardLayerChanged = 1;
        }
    }
}

int Board::distanceAppleSnake()
{
    int appleX(m_currentXApple/m_cellSize);
    int appleY(m_currentYApple/m_cellSize);

    int headX(m_currentXPosition/m_cellSize);
    int headY(m_currentYPosition/m_cellSize);

    int distX(std::min(abs(headX - appleX), m_nbSquaresX - abs(headX - appleX)));
    int distY(std::min(abs(headY - appleY), m_nbSquaresY - abs(headY - appleY)));

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
    for(unsigned int i(0); i < std::min(m_savedXPositionsMod.size() - 1, m_length); i++)
    {
        painter.drawRect(m_rectBody(m_savedXPositionsMod.size() - i - 2,0));
    }

    //dessiner queue mouvante
    if(m_savedXPositionsMod.size() > m_length + 1)
    {
        //painter.setBrush(Qt::blue);
        painter.drawRect(m_rectTail(0));
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

    QRect apple(m_rectApple(0)); //placé au début, on le déplace dans la ligne suivante
    painter.setPen(Qt::NoPen);

    //painter.setBrush(Qt::green);
    painter.setBrush(QColor(std::min(2*(100 - m_pointsCurrentApple),128),
                            std::max(255 - 1*(100 - m_pointsCurrentApple),128),
                            std::min(2*(100 - m_pointsCurrentApple),128)));

    if(m_win == FALSE)
    {
        painter.drawRect(apple);
    }



}


/*
  Painting the whole board
*/
void Board::paintWholeBoard(QPainter &painter)
{
    //setPalette(QPalette(QColor(250, 250, 200)));
    painter.setPen(Qt::NoPen);
    if(m_currentBoard == "density" || m_currentBoard == "edge" || m_currentBoard == "nonoriented" || m_currentBoard == "apple")
    {
        painter.setBrush(Qt::white);
    }
    else
    {
        painter.setBrush(QColor(250, 250, 200));
    }

    painter.drawRect(m_board());
}

/*
  Current layer and painting statistic board layers
*/
// Snake's density layer
void Board::paintDensity(QPainter &painter)
{
    painter.setPen(Qt::NoPen);

    int minTransp(20);
    int maxTransp(240);
    int pas((maxTransp - minTransp)/(m_savedMaxDensityValue + 1));

    for(int y(0); y < m_nbSquaresY; y++)
    {
        for(int x(0); x < m_nbSquaresX; x++)
        {
            painter.setBrush(QColor(0, 0, 255, minTransp + pas*m_savedDensity[y*m_nbSquaresX + x]));
            painter.drawRect(m_rect(x* m_cellSize, y* m_cellSize));
        }
       }
}

// Oriented edges crossed
void Board::paintEdge(QPainter &painter)
{
    painter.setPen(Qt::NoPen);

    int minTransp(20);
    int maxTransp(240);
    int pas((maxTransp - minTransp)/(m_savedMaxEdgeValue + 1));

    for(int y(0); y < m_nbSquaresY; y++)
    {
        for(int x(0); x < m_nbSquaresX; x++)
        {
            painter.setBrush(QColor(255, 0, 0, minTransp + pas*m_savedEastEdges[y*m_nbSquaresX + x]));
            painter.drawPolygon(m_eastTriangle(x,y));

            painter.setBrush(QColor(255, 0, 0, minTransp + pas*m_savedNorthEdges[y*m_nbSquaresX + x]));
            painter.drawPolygon(m_northTriangle(x,y));

            painter.setBrush(QColor(255, 0, 0, minTransp + pas*m_savedWestEdges[y*m_nbSquaresX + x]));
            painter.drawPolygon(m_westTriangle(x,y));

            painter.setBrush(QColor(255, 0, 0, minTransp + pas*m_savedSouthEdges[y*m_nbSquaresX + x]));
            painter.drawPolygon(m_southTriangle(x,y));
        }
    }
}



// Non-oriented edges crossed
void Board::paintNonOriented(QPainter &painter)
{
    painter.setPen(Qt::NoPen);

    int minTransp(20);
    int maxTransp(240);
    int pas((maxTransp - minTransp)/(2*m_savedMaxEdgeValue));

    for(int y(0); y < m_nbSquaresY; y++)
    {
        for(int x(0); x < m_nbSquaresX; x++)
        {
            if(x != 0)
            {
                painter.setBrush(QColor(255, 0, 0, minTransp + pas*(m_savedEastEdges[y*m_nbSquaresX + x] + m_savedWestEdges[y*m_nbSquaresX + (x - 1)])));
                painter.drawPolygon(m_eastTriangle(x,y));

                painter.setBrush(QColor(255, 0, 0, minTransp + pas*(m_savedEastEdges[y*m_nbSquaresX + x] + m_savedWestEdges[y*m_nbSquaresX + (x - 1)])));
                painter.drawPolygon(m_westTriangle(x - 1,y));
            }
            else
            {
                painter.setBrush(QColor(255, 0, 0, minTransp + pas*(m_savedEastEdges[y*m_nbSquaresX + x] + m_savedWestEdges[y*m_nbSquaresX + (x - 1) + m_nbSquaresX])));
                painter.drawPolygon(m_eastTriangle(x,y));

                painter.setBrush(QColor(255, 0, 0, minTransp + pas*(m_savedEastEdges[y*m_nbSquaresX + x] + m_savedWestEdges[y*m_nbSquaresX + (x - 1) + m_nbSquaresX])));
                painter.drawPolygon(m_westTriangle(x - 1 + m_nbSquaresX,y));
            }

            if(y != 0)
            {
                painter.setBrush(QColor(255, 0, 0, minTransp + pas*(m_savedNorthEdges[(y - 1)*m_nbSquaresX + x] + m_savedSouthEdges[y*m_nbSquaresX + x])));
                painter.drawPolygon(m_northTriangle(x,y - 1));

                painter.setBrush(QColor(255, 0, 0, minTransp + pas*(m_savedNorthEdges[(y - 1)*m_nbSquaresX + x] + m_savedSouthEdges[y*m_nbSquaresX + x])));
                painter.drawPolygon(m_southTriangle(x,y));
            }
            else
            {
                painter.setBrush(QColor(255, 0, 0, minTransp + pas*(m_savedNorthEdges[(y - 1 + m_nbSquaresY)*m_nbSquaresX + x] + m_savedSouthEdges[y*m_nbSquaresX + x])));
                painter.drawPolygon(m_northTriangle(x,y - 1 + m_nbSquaresY));

                painter.setBrush(QColor(255, 0, 0, minTransp + pas*(m_savedNorthEdges[(y - 1 + m_nbSquaresY)*m_nbSquaresX + x] + m_savedSouthEdges[y*m_nbSquaresX + x])));
                painter.drawPolygon(m_southTriangle(x,y));
            }
        }
    }
}

// Apple's density layer
void Board::paintApple(QPainter &painter)
{
    painter.setPen(Qt::NoPen);
    int minTransp(20);
    int maxTransp(240);
    int pas((maxTransp - minTransp)/(m_savedMaxAppleValue + 1));
    for(int y(0); y < m_nbSquaresY; y++)
    {
        for(int x(0); x < m_nbSquaresX; x++)
        {
            painter.setBrush(QColor(0, 255, 0, minTransp + pas*m_savedApple[y*m_nbSquaresX + x]));
            painter.drawRect(m_rect(x* m_cellSize, y* m_cellSize));
        }
    }
}

////////////////////////
// Whole game history //
////////////////////////
void Board::saveAllHistory()
{
    m_savedAllSteps.push_back(m_steps);
    m_savedAllScore.push_back(m_score);
    m_savedAllNbApples.push_back(m_savedXPositionsApple.size() - 1);
    m_savedAllSnakeLength.push_back(m_length + 1);
    m_savedAllXPositionsApple.push_back(m_savedXPositionsApple.back());
    m_savedAllYPositionsApple.push_back(m_savedYPositionsApple.back());
}

void Board::writingAllHistory()
{
    QFile data("output.txt");
    if(data.open(QFile::WriteOnly | QIODevice::Append))
    {
        QTextStream out(&data);

        if(m_currentPlayId == 0)
        {
            out << "partieEnCours" << "	" << "temps" << "	" << "score" << "	"
                << "nbapplesMangees" << "	" << "longueurSnake" << "	"
                <<  "positionsX" << "	" << "positionsY" << "	"
                 << "positionsappleX" << "	" << "positionsappleY"
                 << endl;
        }

        for(unsigned int i(0); i < m_savedXPositions.size(); i++)
        {
            out << m_currentPlayId << "	" << m_savedAllSteps[i] << "	" << m_savedAllScore[i] << "	"
                << m_savedAllNbApples[i] << "	" << m_savedAllSnakeLength[i] << "	"
                <<  m_savedXPositions[i] << "	" << m_savedYPositions[i] << "	"
                << m_savedAllXPositionsApple[i] << "	" << m_savedAllYPositionsApple[i]
                << endl;
        }
    }
}
