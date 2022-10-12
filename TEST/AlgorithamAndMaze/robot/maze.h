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

class Maze
{
public:
    short CELLS[ROWS][COLUMNS];
    short Arr[no];

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

            /*for (int i = 0; i < ROWS; i++)
            {
            for (int j = 0; j < COLUMNS; j++)
            {
                cout << CELLS[i][j] << "  |";
            }
            cout << endl;
            }*/
        }
    }

    short *getNeb(short x, short y)
    {
        if (x + 1 < ROWS && CELLS[x + 1][y] == 127)
        {
            arr[0] = x + 1;
            arr[1] = y;
        }
        else
        {
            arr[0] = 127;
            arr[1] = 127;
        }
        if (x - 1 >= 0 && CELLS[x - 1][y] == 127)
        {
            arr[2] = x - 1;
            arr[3] = y;
        }
        else
        {
            arr[2] = 127;
            arr[3] = 127;
        }
        if (y + 1 < COLUMNS && CELLS[x][y + 1] == 127)
        {
            arr[4] = x;
            arr[5] = y + 1;
        }
        else
        {
            arr[4] = 127;
            arr[5] = 127;
        }
        if (y - 1 >= 0 && CELLS[x][y - 1] == 127)
        {
            arr[6] = x;
            arr[7] = y - 1;
        }
        else
        {
            arr[6] = 127;
            arr[7] = 127;
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

                short *neb = getNeb(arr[i], arr[i + 1]);

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
};

#endif
