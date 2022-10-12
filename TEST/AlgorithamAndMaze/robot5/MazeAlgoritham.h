#ifndef MAZE_H
#define MAZE_H

#include <iostream>
using namespace std;

#define arraySize 50

#define rows 10
#define columns 10
#define targetRow 5
#define targetColum 5

int cells[rows][columns];
int quoer[arraySize][2];
int position[2] = {9, 0};
int tempPosition;
int neighbour[4][2];
int minCell[2];
int arrCount = 1;
int stepCount;
int tempCount;
int step = 0;

bool horizontalWalls[rows + 1][columns];
bool verticalWalls[rows][columns];

void initialized()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            cells[i][j] = 127;
        }
    }
    for (int i = 0; i < arraySize; i++)
    {
        quoer[i][0] = 127;
        quoer[i][1] = 127;
    }

    quoer[0][0] = targetRow;
    quoer[0][1] = targetColum;

    for (int i = 0; i < rows + 1; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            horizontalWalls[i][j] = false;
        }
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns + 1; j++)
        {
            verticalWalls[i][j] = false;
        }
    }

    for (int i = 0; i < rows; i++)
    {
        verticalWalls[i][0] = true;
        verticalWalls[i][columns] = true;
    }
    for (int i = 0; i < columns; i++)
    {
        horizontalWalls[0][i] = true;
        horizontalWalls[rows][i] = true;
    }
}

void addVerticalWall(int x, int y)
{
    verticalWalls[x][y] = true;
}

void addHorizontalWall(int x, int y)
{
    horizontalWalls[x][y] = true;
}

void getNeighbour(int x, int y)
{
    for (int i = 0; i < 4; i++)
    {
        neighbour[i][0] = 127;
        neighbour[i][1] = 127;
    }

    if (x + 1 < rows && horizontalWalls[x + 1][y] == false)
    {
        neighbour[0][0] = x + 1;
        neighbour[0][1] = y;
    }
    if (x - 1 >= 0 && horizontalWalls[x][y] == false)
    {
        neighbour[1][0] = x - 1;
        neighbour[1][1] = y;
    }
    if (y + 1 < columns && verticalWalls[x][y + 1] == false)
    {
        neighbour[2][0] = x;
        neighbour[2][1] = y + 1;
    }
    if (y - 1 >= 0 && verticalWalls[x][y] == false)
    {
        neighbour[3][0] = x;
        neighbour[3][1] = y - 1;
    }

    for (int i = 0; i < 4; i++)
    {
        if (neighbour[i][0] != 127)
        {
            minCell[0] = neighbour[i][0];
            minCell[1] = neighbour[i][1];
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (neighbour[i][0] != 127 && (cells[neighbour[i][0]][neighbour[i][1]] < cells[minCell[0]][minCell[1]]))
        {
            minCell[0] = neighbour[i][0];
            minCell[1] = neighbour[i][1];
        }
    }
}

void quoerUpdate()
{
    for (int i = 0; i < arraySize - 1; i++)
    {
        quoer[i][0] = quoer[i + 1][0];
        quoer[i][1] = quoer[i + 1][1];
    }
}

void firstFillMaze()
{
    bool flag = true;
    arrCount = 1;
    stepCount = 1;
    while (flag)
    {
        flag = false;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                if (cells[i][j] == 127)
                {
                    flag = true;
                }
            }
        }

        for (int i = 0; i < stepCount; i++)
        {
            getNeighbour(quoer[i][0], quoer[i][1]);
            for (int j = 0; j < 4; j++)
            {
                if (neighbour[j][0] != 127 && cells[neighbour[j][0]][neighbour[j][1]] == 127)
                {
                    bool flag = true;
                    for (int k = 0; k < arrCount; k++)
                    {
                        if (quoer[k][0] == neighbour[j][0] && quoer[k][1] == neighbour[j][1])
                        {
                            flag = false;
                        }
                    }
                    if (flag)
                    {
                        quoer[arrCount][0] = neighbour[j][0];
                        quoer[arrCount][1] = neighbour[j][1];
                        arrCount++;
                    }
                }
            }
        }

        tempCount = arrCount;

        for (int i = 0; i < stepCount; i++)
        {
            cells[quoer[0][0]][quoer[0][1]] = step;
            quoerUpdate();
            arrCount--;
        }

        tempCount = arrCount;
        stepCount = tempCount;
        step++;
    }

    for (int i = 0; i < arraySize; i++)
    {
        quoer[i][0] = 127;
        quoer[i][1] = 127;
    }
    quoer[0][0] = position[0];
    quoer[0][1] = position[1];
}

void wakeMouse()
{
    for (int i = 0; i < arraySize; i++)
    {
        quoer[i][0] = 127;
        quoer[i][1] = 127;
    }
    quoer[0][0] = position[0];
    quoer[0][1] = position[1];
    arrCount = 0;

    while (quoer[0][0] != 127)
    {
        getNeighbour(quoer[0][0], quoer[0][1]);
        if ((cells[minCell[0]][minCell[1]] > cells[quoer[0][0]][quoer[0][1]]))
        {
            cells[quoer[0][0]][quoer[0][1]] = cells[minCell[0]][minCell[1]] + 1;
            for (int i = 0; i < 4; i++)
            {
                if (neighbour[i][0] != 127)
                {
                    quoer[arrCount + 1][0] = neighbour[i][0];
                    quoer[arrCount + 1][1] = neighbour[i][1];
                    arrCount = arrCount + 1;
                }
            }
        }

        quoerUpdate();
        if (arrCount != 0)
            arrCount--;
        else
            arrCount = 0;
    }

    getNeighbour(position[0], position[1]);
    position[0] = minCell[0];
    position[1] = minCell[1];
}

#endif