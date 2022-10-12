#include <iostream>
#include "maze.h"
#include "mouse.h"

using namespace std;

void printMaze(Maze maze, Mouse mouse)
{
    for (int i = 0; i < ROWS; i++)
    {
        cout << "   ";

        for (int j = 0; j < COLUMNS; j++)
        {
            if (maze.horiWalls[i][j] == true)
                cout << "___ ";
            else
                cout << "- - ";
        }
        cout << endl;

        for (int j = 0; j < COLUMNS; j++)
        {
            if (maze.verWalls[i][j] == true)
                cout << " || " << maze.CELLS[i][j];
            else
                cout << " : " << maze.CELLS[i][j];
            if (mouse.xPos == i && mouse.yPos == j)
                cout << "*";
        }
        cout << endl;
    }
}

int main()
{
    int Rows = ROWS;
    int Columns = COLUMNS;

    Maze maze;
    Mouse mouse(9, 0, &maze);

    /*for (int i = 0; i < ROWS + 1; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            cout << maze.horiWalls[i][j];
        }
        cout << endl;
    }*/

    while (!(mouse.xPos == T_row && mouse.yPos == T_col))
    {
        cout << endl;
        mouse.fill();
        printMaze(maze, mouse);
        //cout << mouse.xPos << "|" << mouse.yPos << "  " << maze.CELLS[mouse.xPos][mouse.yPos] << endl;
    }
}
