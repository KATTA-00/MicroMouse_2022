#include "maze.h"

void setup() {
  byte x = 10;
  Maze m;
  Serial.begin(9600);
  

  for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            Serial.print(" ");
            Serial.print( m.CELLS[i][j]);
            Serial.print(" |");
            
        }
        Serial.println();

}
}

void loop() {
  // put your main code here, to run repeatedly:

}
