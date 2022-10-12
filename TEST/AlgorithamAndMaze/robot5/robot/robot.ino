#include "MazeAlgoritham.h"" 



void setup() {
  Serial.begin(9600);
  initialized();
    firstFillMaze();
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
  
  
}

void loop() {
  while (!(position[0] == targetRow && position[1] == targetColum))
    {
        wakeMouse();
        printMaze();
    }

}

void printMaze(){
  Serial.println();
  for (int i = 0; i < rows; i++)
    {
        Serial.print("  ");
        for (int j = 0; j < columns; j++)
        {
            if (horizontalWalls[i][j]){
              Serial.print(" __ "); 
            }else{
              Serial.print(" -- ");
            }
              
            }
        Serial.println();
        
        for (int j = 0; j < columns; j++)
        {
            if (verticalWalls[i][j]){
            Serial.print(" | ");
            Serial.print( cells[i][j]);
            }else{
              Serial.print(" : ");
              Serial.print( cells[i][j]);
            }
            if (i == position[0] && j == position[1])
            {
                Serial.print("*");
            }
            
        }
        Serial.println();
  
  }}
