#ifndef MOUSE_H
#define MOUSE_H
#include "maze.h"
//#include <iostream>

//using namespace std;

class Mouse
{

public:
    byte xPos;
    byte yPos;
    Maze *maze;
    byte quore[no][2];
    byte quoreCount = 0;
    byte neighbour[4][2];
    byte nextCell[2] = {127, 127};
    byte minCell[2] = {127, 127};

    Mouse(byte x, byte y, Maze *maze)
    {
        this->xPos = x;
        this->yPos = y;
        this->maze = maze;
        //cout << this->maze << endl;

        for (byte i = 0; i < no; i++)
        {
            for (byte j = 0; j < 2; j++)
            {
                this->quore[i][j] = 127;
            }
        }

        this->resetNeighbour();
    }

    void floodFill(byte xPos, byte yPos)
    {

        this->getMinNeighbour(xPos, yPos);
        if ((maze->CELLS[minCell[0]][minCell[1]] > maze->CELLS[xPos][yPos]))
        {
            maze->CELLS[xPos][yPos] = maze->CELLS[minCell[0]][minCell[1]] + 1;

            for (byte i = 0; i < 4; i++)
            {
                if (this->neighbour[i][0] != 127)
                {
                    this->quore[this->quoreCount + 1][0] = this->neighbour[i][0];
                    this->quore[this->quoreCount + 1][1] = this->neighbour[i][1];
                    this->quoreCount = this->quoreCount + 1;
                }
            }
        }
    }

    void fill()
    {
        this->quore[0][0] = xPos;
        this->quore[0][1] = yPos;
        this->quoreCount = 0;

        while (this->quore[0][0] != 127)
        {

            this->floodFill(quore[0][0], quore[0][1]);
            this->resetQuore();
        }

        this->getMinNeighbour(xPos, yPos);
        this->xPos = minCell[0];
        this->yPos = minCell[1];
    }

    void resetQuore()
    {
        for (byte i = 0; i < no - 1; i++)
        {
            this->quore[i][0] = this->quore[i + 1][0];
            this->quore[i][1] = this->quore[i + 1][1];
        }
        if (this->quoreCount != 0)
            this->quoreCount--;
        else
            this->quoreCount = 0;
    }

    void resetNeighbour()
    {
        for (byte i = 0; i < 4; i++)
        {
            for (byte j = 0; j < 2; j++)
            {
                this->neighbour[i][j] = 127;
            }
        }
    }

    void getMinNeighbour(byte xPos, byte yPos)
    {
        this->resetNeighbour();
        if ((xPos + 1 < ROWS) && maze->horiWalls[xPos + 1][yPos] == false)
        {
            this->neighbour[0][0] = xPos + 1;
            this->neighbour[0][1] = yPos;
        }
        if ((xPos - 1 >= 0) && maze->horiWalls[xPos][yPos] == false)
        {
            this->neighbour[1][0] = xPos - 1;
            this->neighbour[1][1] = yPos;
        }
        if ((yPos + 1 < COLUMNS) && maze->verWalls[xPos][yPos + 1] == false)
        {
            this->neighbour[2][0] = xPos;
            this->neighbour[2][1] = yPos + 1;
        }
        if ((xPos - 1 >= 0) && maze->verWalls[xPos][yPos] == false)
        {
            this->neighbour[3][0] = xPos;
            this->neighbour[3][1] = yPos - 1;
        }

        for (byte i = 0; i < 4; i++)
        {
            if (this->neighbour[i][0] != 127)
            {
                this->minCell[0] = this->neighbour[i][0];
                this->minCell[1] = this->neighbour[i][1];
            }
        }

        for (byte i = 0; i < 4; i++)
        {
            if (this->neighbour[i][0] != 127 && (maze->CELLS[this->neighbour[i][0]][this->neighbour[i][1]] < maze->CELLS[this->minCell[0]][this->minCell[1]]))
            {
                this->minCell[0] = this->neighbour[i][0];
                this->minCell[1] = this->neighbour[i][1];
            }
        }
    }
};

#endif
