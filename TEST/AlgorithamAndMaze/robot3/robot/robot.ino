#include "maze.h"
#include "mouse.h"

Maze m;
Mouse mouse(9,0,&m);
void setup() {
  Serial.begin(9600);
  print(&m, &mouse);
  byte x = 10;
  
}

void loop() {
  while (!(mouse.xPos == T_row && mouse.yPos == T_col))
    {
        mouse.fill();
        print(&m, &mouse);
        //cout << mouse.xPos << "|" << mouse.yPos << "  " << maze.CELLS[mouse.xPos][mouse.yPos] << endl;
        delay(10);
    }

}

void print(Maze* m,Mouse* mouse){
  Serial.println();
  for (int i = 0; i < ROWS; i++)
    {
        Serial.print("  ");
        for (int j = 0; j < COLUMNS; j++)
        {
            if (m->horiWalls[i][j]){
              Serial.print(" __ "); 
            }else{
              Serial.print(" -- ");
            }
              
            }
        Serial.println();
        
        for (int j = 0; j < COLUMNS; j++)
        {
            if (m->verWalls[i][j]){
            Serial.print(" | ");
            Serial.print( m->CELLS[i][j]);
            }else{
              Serial.print(" : ");
              Serial.print( m->CELLS[i][j]);
            }
            if (mouse->xPos == i && mouse->yPos == j){
              Serial.print("*");
            }
            
        }
        Serial.println();
  
  }}
