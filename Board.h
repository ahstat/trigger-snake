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
        /*********************
        * Private attributes *
        *********************/
        ////
        // QTimer
        ////
        //The QTimer class provides ''repetitive'' and single-shot timers.
        QTimer *m_autoMoveTimer;

        ////
        // General characteristics
        ////
        // speed of game (in ms)
        int m_timerSpeed;
        // number of cells in X
        int m_nbSquaresX;
        // number of cells in Y
        int m_nbSquaresY;
        // size of a cell in pixels ; each cells has area of m_cellSize^2
        int m_cellSize;
        // score of the current game
        int m_score;
        // previous hi-score
        int m_hiScore;
        // current play number
        int m_currentPlayId;
        // Whether it is the end of current game
        bool m_endOfGame;
        // Whether player has win
        bool m_win;

        ////
        // Information on the snake
        ////
        // direction: 0=east ; 1=north ; 2=west ; 3=south ; -1=undefined
        int m_direction;
        // previous direction: in 0, 1, 2, 3, -1, as for m_direction
        int m_previousDirection;

        // this boolean helps to constraint the snake to stay on the grid
        // When a new direction is set and m_repositionOnGrid == TRUE, the
        // snake will wait to be on a complete cell before moving
        bool m_repositionOnGrid;
        // current X position in pixels in [0, m_cellSize * m_nbSquaresX[
        int m_currentXPosition;
        // current Y position in pixels in [0, m_cellSize * m_nbSquaresY[
        int m_currentYPosition;

        // length of the snake: 0=only the head, 1=2 cells, 2=3 cells etc.
        unsigned int m_length;
        // how long the snake has crawled (in number of cells)
        int m_steps;

        ////
        // Information on the apple
        ////
        // current X apple in pixels in [0, m_cellSize * m_nbSquaresX[
        int m_currentXApple;
        // current Y position in pixels in [0, m_cellSize * m_nbSquaresY[
        int m_currentYApple;

        ////
        // Relations between apple and snake
        ////
        // number of points of the current apple
        // set at 100 for a fresh apple, and slowly decrease to 1
        int m_pointsCurrentApple;
        // number of steps we let before apple begins to rot
        // set as distance between snake and apple when the apple appears
        int m_nbStepsBeforeDecreasingPoints;

        // deque of number of steps before increasing the snake
        /*
          Example: snake has length 5, it eats apple, we push 5+1 to the deque
          Two steps after, snake has length 6, it eats apple, we push 6+1
          At each step of the snake, all elements of deque decrease by 1
          When the front element reaches 0, we pop it and increase the snake
          In our example:
          date t: [6]; t+1: [5]; t+2: [4 7]; t+3: [3 6]; t+4: [2 5];
          t+5: [1 6]; t+6: [5] and snake size has increased; t+7: [4], etc.
        */
        std::deque<unsigned int> m_nbStepsBeforeIncreasingSnake;

        // number of pixels for which the snake's tail do not move
        /*
          when the snake grows, the tail must not move for m_cellSize pixels
          m_nbPixDoNotMoveTail is the number of pixels remaining before moving
          the tail again.
          m_nbPixDoNotMoveTail is normally 0, and increase by m_cellSize when
          we begin to increase snake's size
        */
        unsigned int m_nbPixDoNotMoveTail;

        ////
        // Board layers
        ////
        // Current board layer:
        // "" for the classic board layer,
        // "density" for density of snake in each cell,
        // "edge" for density of snake in each edge,
        // "nonoriented" for density of snake in each non-oriented edge,
        // "apple" for density of apple in each cell.
        std::string m_currentBoard;

        // Whether the board layer has changed
        bool m_isBoardLayerChanged;

        ////
        // Saving partial states of the game
        ////
        // save of absolute X-positions of the snake, each in ]-Inf, +Inf[
        // i.e. we do not reset counter when crossing the torus
        // 1 element for 1 step
        std::vector<int> m_savedXPositions;
        // save of absolute Y-positions of the snake, each in ]-Inf, +Inf[
        // 1 element for 1 step
        std::vector<int> m_savedYPositions;

        // save of X-positions of the snake, each in [0, m_nbSquaresX-1]
        // 1 element for 1 step
        std::vector<int> m_savedXPositionsMod;
        // save of Y-positions of the snake, each in [0, m_nbSquaresY-1]
        // 1 element for 1 step
        std::vector<int> m_savedYPositionsMod;

        // save of directions, each in {"north", "east", "south", "west"}
        // 1 element for 1 change in direction
        std::vector<std::string> m_savedDirections;

        // save of X-positions of apple, each in [0, m_nbSquaresX-1]
        // 1 element for 1 new apple
        std::vector<int> m_savedXPositionsApple;
        // save of Y-positions of apple, each in [0, m_nbSquaresY-1]
        // 1 element for 1 new apple
        std::vector<int> m_savedYPositionsApple;

        ////
        // Saving additional states for layers
        ////
        // number of times the snake has crossed each cell of the grid,
        // cells are indexed line by line from 0 to m_nbSquaresX*m_nbSquaresY-1
        std::vector<int> m_savedDensity;

        // number of times the snake has crossed each cell from the north,
        // (resp. east, south, west),
        // cells are indexed line by line from 0 to m_nbSquaresX*m_nbSquaresY-1
        std::vector<int> m_savedNorthEdges;
        std::vector<int> m_savedEastEdges;
        std::vector<int> m_savedSouthEdges;
        std::vector<int> m_savedWestEdges;

        // number of times an apple has appeared in each cell of the grid,
        // cells are indexed line by line from 0 to m_nbSquaresX*m_nbSquaresY-1
        std::vector<int> m_savedApple;

        // maximum element of m_savedDensity
        int m_savedMaxDensityValue;
        // maximum element of the union of the 4 saved edges vectors
        int m_savedMaxEdgeValue;
        // maximum element of m_savedApple
        int m_savedMaxAppleValue;

        ////
        // Saving whole game history (for export)
        ////
        /*
          Each vector has size T
          (T: number of moves/steps of the snake from beginning of this game).
          This information is only used to output a txt file containing whole
          history.
        */
        // Steps 0, 1, ..., T-1
        std::vector<int> m_savedAllSteps;
        // Score at each step t
        std::vector<int> m_savedAllScore;
        // Number of apples eaten at each step t
        std::vector<int> m_savedAllNbApples;
        // Length of snake at each step t
        std::vector<int> m_savedAllSnakeLength;
        // Position X of apples at each step t (int in [0, m_nbSquaresX-1]}
        std::vector<int> m_savedAllXPositionsApple;
        // Position Y of apples at each step t (int in [0, m_nbSquaresY-1]}
        std::vector<int> m_savedAllYPositionsApple;

        /******************
        * Private methods *
        ******************/
        ////
        // General behavior of the game
        ////
        void newGame();
        void begin(); //active le timer de déplacement
        void endOfGame();
        void win();

        // For debugging, print rectangle position in console
        void display(bool isToBeDiplayed, std::string objName, QRect result) const;

        void writingHiScore();

        ////
        // Snake behavior
        ////
        void motion();
        void motionOneStep(); //avance d'un pixel dans le direction en cours (modulo le plateau)

        bool collisionWithSnake(int newX, int newY);

        ////
        // Apple behavior
        ////
        void newApplePosition();
        void decreasingPoints();

        ////
        // Relations between apple and snake behaviors
        ////
        int distanceAppleSnake(); //distance de Manhattan (modulo les bords)

        void testCollision(); //vérification si on touche le snake après avoir avancé. Si c'est le cas, on lance ecrire()
        bool collisionWithApple();
        void eatenApple();

        void planAnIncreaseOfTheSnake(); //augmente la longueur de 1
        void increaseSnake(); //test s'il faut augmenter maintenant le snake

        ////
        // Saving partial states of the game
        ////
        void savePositions(); //enregistre position en cours dans m_savedXPositions/Y
        void savePositionsMod(); //idem modulo la taille du plateau (méthode à lancer après savePositions())

        ////
        // Saving additional states for layers
        ////
        void saveDensity();
        void saveEdge();
        void saveApple();

        ////
        // Saving whole game history (for export)
        ////
        // Update previous vectors by adding current state
        void saveAllHistory();

        // Writting previous vectors into a txt file
        void writingAllHistory();

        /*********
        * Events *
        *********/
        // Key press event
        void keyPressEvent(QKeyEvent *event);
        // Paint event
        void paintEvent(QPaintEvent *event);



        /******************
        * Graphical areas *
        ******************/
        QRect m_board();
        /*
          Selecting areas
        */
        // Generic rectangle of size m_cell * m_cell from (x, y)
        QRect m_rect(int x, int y);

        ////
        // Area for the snake
        ////
        // Where is the head of the snake?
        QRect m_rectHeadUsual(bool isToBeDiplayed);
        // Where is the head while crossing a torus' corner?
        QRect m_rectHeadCorner(bool isToBeDiplayed);
        // m_rectHeadUsual union m_rectHeadCorner forms the complete head region
        QRegion m_rectHead();
        // Where is the i-th component of the snake's body? (from i = 1)
        QRect m_rectBody(unsigned int i, bool isToBeDiplayed);
        // Where is the tail of the snake?
        QRect m_rectTail(bool isToBeDiplayed);

        ////
        // Area for the apples
        ////
        // Where is the current apple?
        QRect m_rectApple(bool isToBeDiplayed);

        QRegion m_regionSnake();
        QRegion m_regionApple();

        QPolygon m_northTriangle(int x, int y);
        QPolygon m_southTriangle(int x, int y);
        QPolygon m_eastTriangle(int x, int y);
        QPolygon m_westTriangle(int x, int y);


        /***********
        * Painting *
        ***********/
        /*
          Painting main layer
        */
        void paintShot(QPainter &painter);
        // When m_isBoardLayerChanged==1, we redraw the whole board
        void paintWholeBoard(QPainter &painter);


        ////
        // Board layers
        ////

        // Painting snake's density layer
        void paintDensity(QPainter &painter);
        // Painting oriented edges crossed
        void paintEdge(QPainter &painter);
        // Painting non-oriented edges crossed
        void paintNonOriented(QPainter &painter);
        // Painting apple's density layer
        void paintApple(QPainter &painter);


};

#endif
