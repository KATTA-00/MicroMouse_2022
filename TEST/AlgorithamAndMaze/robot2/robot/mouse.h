#ifndef MOUSE_H
#define MOUSE_H
#include "maze.h"
//#include <iostream>

//using namespace std;

class Mouse
{

public:
    short xPos;
    short yPos;
    Maze maze;
    short quore[no][2];
    short quoreCount = 0;
    short neighbour[4][2];
    short nextCell[2] = {127, 127};
    short minCell[2] = {127, 127};

    Mouse(short x, short y, Maze *maze)
    {
        this->xPos = x;
        this->yPos = y;
        this->maze = *maze;

        for (short i = 0; i < no; i++)
        {
            for (short j = 0; j < 2; j++)
            {
                quore[i][j] = 127;
            }
        }

        resetNeighbour();
    }

    void floodFill(short xPos, short yPos)
    {

        getMinNeighbour(xPos, yPos);
        if ((maze.CELLS[minCell[0]][minCell[1]] > maze.CELLS[xPos][yPos]))
        {
            maze.CELLS[xPos][yPos] = maze.CELLS[minCell[0]][minCell[1]] + 1;

            for (short i = 0; i < 4; i++)
            {
                if (neighbour[i][0] != 127)
                {
                    quore[quoreCount + 1][0] = neighbour[i][0];
                    quore[quoreCount + 1][1] = neighbour[i][1];
                    quoreCount = quoreCount + 1;
                }
            }
        }
    }

    void fill()
    {
        quore[0][0] = xPos;
        quore[0][1] = yPos;
        quoreCount = 0;

        while (quore[0][0] != 127)
        {

            floodFill(quore[0][0], quore[0][1]);
            resetQuore();
        }

        getMinNeighbour(xPos, yPos);
        xPos = minCell[0];
        yPos = minCell[1];
    }

    void resetQuore()
    {
        for (short i = 0; i < no - 1; i++)
        {
            quore[i][0] = quore[i + 1][0];
            quore[i][1] = quore[i + 1][1];
        }
        if (quoreCount != 0)
            quoreCount--;
        else
            quoreCount = 0;
    }

    void resetNeighbour()
    {
        for (short i = 0; i < 4; i++)
        {
            for (short j = 0; j < 2; j++)
            {
                neighbour[i][j] = 127;
            }
        }
    }

    void getMinNeighbour(short xPos, short yPos)
    {
        resetNeighbour();
        if ((xPos + 1 < ROWS) && maze.horiWalls[xPos + 1][yPos] == false)
        {
            neighbour[0][0] = xPos + 1;
            neighbour[0][1] = yPos;
        }
        if ((xPos - 1 >= 0) && maze.horiWalls[xPos][yPos] == false)
        {
            neighbour[1][0] = xPos - 1;
            neighbour[1][1] = yPos;
        }
        if ((yPos + 1 < COLUMNS) && maze.verWalls[xPos][yPos + 1] == false)
        {
            neighbour[2][0] = xPos;
            neighbour[2][1] = yPos + 1;
        }
        if ((xPos - 1 >= 0) && maze.verWalls[xPos][yPos] == false)
        {
            neighbour[3][0] = xPos;
            neighbour[3][1] = yPos - 1;
        }

        for (short i = 0; i < 4; i++)
        {
            if (neighbour[i][0] != 127)
            {
                minCell[0] = neighbour[i][0];
                minCell[1] = neighbour[i][1];
            }
        }

        for (short i = 0; i < 4; i++)
        {
            if (neighbour[i][0] != 127 && (maze.CELLS[neighbour[i][0]][neighbour[i][1]] < maze.CELLS[minCell[0]][minCell[1]]))
            {
                minCell[0] = neighbour[i][0];
                minCell[1] = neighbour[i][1];
            }
        }
    }
};

#endif
