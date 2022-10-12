int ROWS = 16;
    int COLUMNS = 16;
    int no = 50;
    int = T_row 5;
    int = T_col 5;
    static int arr[8];
    static int tempArr[no];

class Maze
{
public:
    
    int CELLS[ROWS][COLUMNS];
    int Arr[no];

    Maze()
    {
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLUMNS; j++)
            {
                CELLS[i][j] = 255;
            }
        }

        for (int i = 0; i < no; i++)
        {
            Arr[i] = 255;
        }

        Arr[0] = T_row;
        Arr[1] = T_col;
        int flag = 0;
        int count = 0;
        while (flag != 1)
        {
            flag = 1;
            for (int i = 0; i < ROWS; i++)
            {
                for (int j = 0; j < COLUMNS; j++)
                {
                    if (CELLS[i][j] == 255)
                    {
                        flag = 0;
                    }
                }
            }

            for (int i = 0; i < no; i = i + 2)
            {
                if (CELLS[Arr[i]][Arr[i + 1]] == 255 && Arr[i] != 255)
                    CELLS[Arr[i]][Arr[i + 1]] = count;
            }

            count++;
            int *arra = getNedArr(Arr);

            for (int i = 0; i < no; i++)
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

    int *getNeb(int x, int y)
    {
        if (x + 1 < ROWS && CELLS[x + 1][y] == 255)
        {
            arr[0] = x + 1;
            arr[1] = y;
        }
        else
        {
            arr[0] = 255;
            arr[1] = 255;
        }
        if (x - 1 >= 0 && CELLS[x - 1][y] == 255)
        {
            arr[2] = x - 1;
            arr[3] = y;
        }
        else
        {
            arr[2] = 255;
            arr[3] = 255;
        }
        if (y + 1 < COLUMNS && CELLS[x][y + 1] == 255)
        {
            arr[4] = x;
            arr[5] = y + 1;
        }
        else
        {
            arr[4] = 255;
            arr[5] = 255;
        }
        if (y - 1 >= 0 && CELLS[x][y - 1] == 255)
        {
            arr[6] = x;
            arr[7] = y - 1;
        }
        else
        {
            arr[6] = 255;
            arr[7] = 255;
        }

        return arr;
    }

    int *getNedArr(int arr[])
    {
        for (int i = 0; i < no; i++)
        {
            tempArr[i] = 255;
        }

        int n = 0;
        for (int i = 0; i < no; i += 2)
        {
            if (arr[i] != 255)
            {

                int *neb = getNeb(arr[i], arr[i + 1]);

                for (int x = 0; x < 8; x += 2)
                {
                    if (neb[x] != 255)
                    {
                        bool flag = true;
                        for (int i = 0; i < no; i += 2)
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
