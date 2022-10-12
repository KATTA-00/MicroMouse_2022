#ifndef MAZE_H
#define MAZE_H
//#include <iostream>

//using namespace std;

#define no 50

#define ROWS 16
#define COLUMNS 16
#define T_row 5
#define T_col 5

class Maze
{
public:
    byte CELLS[ROWS][COLUMNS];
    byte Arr[no];

    byte horiWalls[ROWS + 1][COLUMNS];
    byte verWalls[ROWS][COLUMNS + 1];

    byte neb[8];
    byte tempArr[no];
    byte n;
    byte count;

    Maze()
    {
        for (byte i = 0; i < ROWS; i++)
        {
            for (byte j = 0; j < COLUMNS; j++)
            {
                this->CELLS[i][j] = 127;
            }
        }

        for (byte i = 0; i < no; i++)
        {
            this->Arr[i] = 127;
        }

        this->Arr[0] = T_row;
        this->Arr[1] = T_col;

        this->makeWalls();
        this->solveBegin();
        
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

    void getNeb(byte x, byte y)
    {
        for (byte i = 0; i < 8; i++)
        {
            this->neb[i] = 127;
        }

        if (x + 1 < ROWS && CELLS[x + 1][y] == 127)
        {
            this->neb[0] = x + 1;
            this->neb[1] = y;
        }

        if (x - 1 >= 0 && CELLS[x - 1][y] == 127)
        {
            this->neb[2] = x - 1;
            this->neb[3] = y;
        }

        if (y + 1 < COLUMNS && CELLS[x][y + 1] == 127)
        {
            this->neb[4] = x;
            this->neb[5] = y + 1;
        }
        if (y - 1 >= 0 && CELLS[x][y - 1] == 127)
        {
            this->neb[6] = x;
            this->neb[7] = y - 1;
        }

    }

    void getNedArr()
    {
        for (byte i = 0; i < no; i++)
        {
            this->tempArr[i] = 127;
        }

        this->n = 0;
        for (byte i = 0; i < no; i += 2)
        {
            if (this->Arr[i] != 127)
            {

                this->getNeb(this->Arr[i], this->Arr[i + 1]);

                for (byte x = 0; x < 8; x += 2)
                {
                    if (this->neb[x] != 127)
                    {
                        bool flag = true;
                        for (byte i = 0; i < no; i += 2)
                        {
                            if (this->tempArr[i] == this->neb[x] && this->tempArr[i + 1] == this->neb[x + 1])
                                flag = false;
                        }
                        if (flag)
                        {
                            this->tempArr[n] = this->neb[x];
                            this->tempArr[n + 1] = this->neb[x + 1];
                            n += 2;
                        }
                    }
                }
            }
        }
    }

    void solveBegin()
    {
        bool flag = true;
        count = 0;
        while (flag)
        {
            flag = false;
            for (byte i = 0; i < ROWS; i++)
            {
                for (byte j = 0; j < COLUMNS; j++)
                {
                    if (this->CELLS[i][j] == 127)
                    {
                        flag = true;
                    }
                }
            }

            for (byte i = 0; i < no; i = i + 2)
            {
                if (this->CELLS[this->Arr[i]][this->Arr[i + 1]] == 127 && this->Arr[i] != 127)
                    this->CELLS[this->Arr[i]][this->Arr[i + 1]] = count;
            }
            count++;
            this->getNedArr();
            for (byte i = 0; i < no; i++)
            {
                this->Arr[i] = this->tempArr[i];
            }
        }
    }

    void makeWalls()
    {
        for (byte i = 0; i < ROWS + 1; i++)
        {
            for (byte j = 0; j < COLUMNS; j++)
            {
                this->horiWalls[i][j] = false;
            }
        }
        for (byte i = 0; i < ROWS; i++)
        {
            for (byte j = 0; j < COLUMNS + 1; j++)
            {
                this->verWalls[i][j] = false;
            }
        }

        for (byte i = 0; i < ROWS; i++)
        {
            this->verWalls[i][0] = true;
            this->verWalls[i][COLUMNS] = true;
        }
        for (byte i = 0; i < COLUMNS; i++)
        {
            this->horiWalls[0][i] = true;
            this->horiWalls[ROWS][i] = true;
        }
    }

    void addVerticalWall(byte x, byte y)
    {
        this->verWalls[x][y] = true;
    }

    void addHorizontalWall(byte x, byte y)
    {
        this->horiWalls[x][y] = true;
    }
};

#endif
