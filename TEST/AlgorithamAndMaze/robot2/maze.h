#ifndef MAZE_H
#define MAZE_H
//#include <iostream>

//using namespace std;

#define no 50

#define ROWS 10
#define COLUMNS 10
#define T_row 5
#define T_col 5

static short arr[8];
static short tempArr[no];
short *neb;

class Maze
{
public:
    short CELLS[ROWS][COLUMNS];
    short Arr[no];

    short horiWalls[ROWS + 1][COLUMNS];
    short verWalls[ROWS][COLUMNS + 1];

    Maze()
    {
        for (short i = 0; i < ROWS; i++)
        {
            for (short j = 0; j < COLUMNS; j++)
            {
                CELLS[i][j] = 127;
            }
        }

        for (short i = 0; i < no; i++)
        {
            Arr[i] = 127;
        }

        Arr[0] = T_row;
        Arr[1] = T_col;

        makeWalls();
        solveBegin();
        addVerticalWall(9, 1);
        addVerticalWall(8, 1);
        addVerticalWall(7, 1);
        addVerticalWall(6, 1);
        addVerticalWall(5, 1);
        addVerticalWall(4, 1);
        addVerticalWall(3, 1);
        addHorizontalWall(4, 3);
        addHorizontalWall(4, 4);
        addHorizontalWall(4, 5);
    }

    short *getNeb(short x, short y)
    {
        for (short i = 0; i < 8; i++)
        {
            arr[i] = 127;
        }

        if (x + 1 < ROWS && CELLS[x + 1][y] == 127)
        {
            arr[0] = x + 1;
            arr[1] = y;
        }

        if (x - 1 >= 0 && CELLS[x - 1][y] == 127)
        {
            arr[2] = x - 1;
            arr[3] = y;
        }

        if (y + 1 < COLUMNS && CELLS[x][y + 1] == 127)
        {
            arr[4] = x;
            arr[5] = y + 1;
        }
        if (y - 1 >= 0 && CELLS[x][y - 1] == 127)
        {
            arr[6] = x;
            arr[7] = y - 1;
        }

        return arr;
    }

    short *getNedArr(short arr[])
    {
        for (short i = 0; i < no; i++)
        {
            tempArr[i] = 127;
        }

        short n = 0;
        for (short i = 0; i < no; i += 2)
        {
            if (arr[i] != 127)
            {

                neb = getNeb(arr[i], arr[i + 1]);

                for (short x = 0; x < 8; x += 2)
                {
                    if (neb[x] != 127)
                    {
                        bool flag = true;
                        for (short i = 0; i < no; i += 2)
                        {
                            if (tempArr[i] == neb[x] && tempArr[i + 1] == neb[x + 1])
                                flag = false;
                        }
                        if (flag)
                        {
                            tempArr[n] = neb[x];
                            tempArr[n + 1] = neb[x + 1];
                            n += 2;
                        }
                    }
                }
            }
        }
        return tempArr;
    }

    void solveBegin()
    {
        bool flag = true;
        short count = 0;
        while (flag)
        {
            flag = false;
            for (short i = 0; i < ROWS; i++)
            {
                for (short j = 0; j < COLUMNS; j++)
                {
                    if (CELLS[i][j] == 127)
                    {
                        flag = true;
                    }
                }
            }

            for (short i = 0; i < no; i = i + 2)
            {
                if (CELLS[Arr[i]][Arr[i + 1]] == 127 && Arr[i] != 127)
                    CELLS[Arr[i]][Arr[i + 1]] = count;
            }
            count++;
            short *arra = getNedArr(Arr);
            for (short i = 0; i < no; i++)
            {
                Arr[i] = arra[i];
            }
        }
    }

    void makeWalls()
    {
        for (short i = 0; i < ROWS + 1; i++)
        {
            for (short j = 0; j < COLUMNS; j++)
            {
                horiWalls[i][j] = false;
            }
        }
        for (short i = 0; i < ROWS; i++)
        {
            for (short j = 0; j < COLUMNS + 1; j++)
            {
                verWalls[i][j] = false;
            }
        }

        for (short i = 0; i < ROWS; i++)
        {
            verWalls[i][0] = true;
            verWalls[i][COLUMNS] = true;
        }
        for (short i = 0; i < COLUMNS; i++)
        {
            horiWalls[0][i] = true;
            horiWalls[ROWS][i] = true;
        }
    }

    void addVerticalWall(short x, short y)
    {
        verWalls[x][y] = true;
    }

    void addHorizontalWall(short x, short y)
    {
        horiWalls[x][y] = true;
    }
};

#endif