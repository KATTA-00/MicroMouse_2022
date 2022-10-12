#include <iostream>
#include "MazeAlgoritham.h"

using namespace std;

void printMaze()
{
    for (int i = 0; i < rows; i++)
    {
        cout << "   ";

        for (int j = 0; j < columns; j++)
        {
            if (horizontalWalls[i][j] == true)
                cout << "___ ";
            else
                cout << "- - ";
        }
        cout << endl;

        for (int j = 0; j < columns; j++)
        {
            if (verticalWalls[i][j] == true)
                cout << " || " << cells[i][j];
            else
                cout << " : " << cells[i][j];
            if (i == position[0] && j == position[1])
            {
                cout << "*";
            }
        }
        cout << endl;
    }
}

int main()
{

    initialized();
    firstFillMaze();
    printMaze();

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
    addHorizontalWall(4, 2);
    addHorizontalWall(4, 1);
    addHorizontalWall(4, 6);
    addVerticalWall(3, 6);

    while (!(position[0] == targetRow && position[1] == targetColum))
    {
        wakeMouse();
        printMaze();
    }
}