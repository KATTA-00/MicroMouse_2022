   #define ROWS 8 
   #define COLUMNS 8
   
   int targetRow = 4;
   int targetColumn = 3;
   
   #define NORTH 0
   #define EAST 1
   #define SOUTH 2
   #define WEST 3
     
   int mouseRow;
   int mouseColumn;
   int mouseHeading;
   
   int values[ROWS][COLUMNS]={};
   
   // vertical walls array
   bool verticalWalls[ROWS][COLUMNS + 1];

   // horizontal walls array
   bool horizontalWalls[ROWS + 1][COLUMNS];

   // vertical mind walls array
   bool verticalMindWalls[ROWS][COLUMNS + 1];

   // horizontal mind walls array
   bool horizontalMindWalls[ROWS + 1][COLUMNS];
 
   void NanoMouseMaze()
    {
      //initialize verticalWalls (add exterior walls)
      for (int i = 0; i < ROWS; i++)
      {
        for (int j = 0; j < COLUMNS + 1; j++)
        {
          if (j == 0 || j == COLUMNS)
          {
            verticalWalls[i][j] = true;
          }
        }
      }

      //initialize horizontalWalls (add exterior walls)
      for (int i = 0; i < ROWS + 1; i++)
      {
        for (int j = 0; j < COLUMNS; j++)
        {
          if (i == 0 || i == ROWS)
          {
            horizontalWalls[i][j] = true;
          }
        }
      }
    }
    
    void addwalls(int dir) {
      if (dir == 0 || dir == 4) {
        horizontalWalls[mouseRow][mouseColumn] = true;
      }
      else if (dir == 1) {
        verticalWalls[mouseRow][mouseColumn + 1] = true;
      }
      else if (dir == 2) {
        horizontalWalls[mouseRow + 1][mouseColumn] = true;
      }
      else if (dir == 3 || dir == -1) {
        verticalWalls[mouseRow][mouseColumn] = true;
      }

    }
    
    
    void solve() {
      for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
          if (i == targetRow && j == targetColumn) {
            values[i][j] = 0;
          }
          else {
            values[i][j] = 255;
          }
        }
      }
      int count = 1;
      int foold_val = 0;
      while (count != 0) {
        count = 0;
        for (int i = 0; i < ROWS; i++) {
          for (int j = 0; j < COLUMNS; j++) {
            if (values[i][j] == foold_val) {
              if (horizontalWalls[i][j] == false && values[i - 1][j] > foold_val && (i - 1) >= 0) {
                values[i - 1][j] = foold_val + 1;
              }
              if (horizontalWalls[i + 1][j] == false && values[i + 1][j] > foold_val && (i + 1) < ROWS) {
                values[i + 1][j] = foold_val + 1;
              }

              if (verticalWalls[i][j] == false && values[i][j - 1] > foold_val && (j - 1) >= 0) {
                values[i][j - 1] = foold_val + 1;
              }
              if (verticalWalls[i][j + 1] == false && values[i][j + 1] > foold_val && (j + 1) < ROWS) {
                values[i][j + 1] = foold_val + 1;
              }
            }
            else if (values[i][j] > foold_val) {
              count++;
            }
          }
        }
        foold_val++;
      }
    }
    
    void print()
    {
      for (int i = 0; i < 2 * ROWS + 1; i++)
      {
        for (int j = 0; j < 2 * COLUMNS + 1; j++)
        {
          //Add Horizontal Walls
          if (i % 2 == 0 && j % 2 == 1)
          {
            if (horizontalWalls[i / 2][j / 2] == true)
            {
              printf(" ---");
            }
            else
            {
              printf("    ");
            }
          }

          //Add Vertical Walls
          if (i % 2 == 1 && j % 2 == 0)
          {
            if (verticalWalls[i / 2][j / 2] == true)
            {
              printf("|");
            }
            else
            {
              printf(" ");///////////////
            }
          }
          
          //Add Flood Fill Values
          if (i % 2 == 1 && j % 2 == 1)
          {
            if ((i - 1) / 2 == mouseRow && (j - 1) / 2 == mouseColumn)
            {
              if (mouseHeading == NORTH)
              {
                printf(" ^ ");
              }
              else if (mouseHeading == EAST)
              {
                printf(" > ");
              }
              else if (mouseHeading == SOUTH)
              {
                printf(" v ");
              }
              else if (mouseHeading == WEST)
              {
                printf(" < ");
              }
            }
            else
            {
              int value = values[(i - 1) / 2][(j - 1) / 2];
              if (value >= 100)
              {
                printf("%d",value);
              }
              else
              {
                printf(" ");
                printf("%d",value);
              }
              if (value < 10)
              {
                printf(" ");
              }
            }
          }

          
        }
        printf("\n");
      }
      printf("\n");
    }