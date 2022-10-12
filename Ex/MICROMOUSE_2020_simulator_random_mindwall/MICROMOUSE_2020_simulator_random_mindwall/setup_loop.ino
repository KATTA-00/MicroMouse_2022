void setup() {
  Serial.begin(9600);

  pinMode(LM, OUTPUT);
  pinMode(RM, OUTPUT);
  pinMode(leftF, OUTPUT);
  pinMode(leftB, OUTPUT);
  pinMode(rightF, OUTPUT);
  pinMode(rightB, OUTPUT);

  mouseRow = init_mouseRow;
  mouseColumn = init_mouseColumn;
  mouseHeading = init_mouseHeading;

  cal();
  test();
}

void loop() {
  ///////////////////////////////////////////////////////////////////////////////////////////////start Heading to the target////////////////////////////////////////////////
  while (mouseRow != maze.targetRow || mouseColumn != maze.targetColumn) {
    check();

    if (condition == 0) {
      if(jun_cnt==1){
        maze.addMindwalls(mouseHeading-2);
      }
      int rand_A = random(0, 3);
      if (mouseHeading == 0) {
        if (rand_A == 0) {
          mouseColumn--;
          mouseHeading = 3;
        }
        else if (rand_A == 1) {
          mouseRow--;
        }
        else if (rand_A == 2) {
          mouseColumn++;
          mouseHeading = 1;
        }
      }
      else if (mouseHeading == 1) {
        if (rand_A == 0) {
          mouseRow--;
          mouseHeading = 0;
        }
        else if (rand_A == 1) {
          mouseColumn++;
        }
        else if (rand_A == 2) {
          mouseRow++;
          mouseHeading = 2;

        }
      }
      else if (mouseHeading == 2) {
        if (rand_A == 0) {
          mouseColumn++;
          mouseHeading = 1;
        }
        else if (rand_A == 1) {
          mouseRow++;
        }
        else if (rand_A == 2) {
          mouseColumn--;
          mouseHeading = 3;
        }
      }
      else if (mouseHeading == 3) {
        if (rand_A == 0) {
          mouseRow++;
          mouseHeading = 2;
        }
        else if (rand_A == 1) {
          mouseColumn--;
        }
        else if (rand_A == 2) {
          mouseRow--;
          mouseHeading = 0;
        }
      }
    }
    //////////////////////////////////////////////////////
    else if (condition == 1) {
      if(jun_cnt==1){
        maze.addMindwalls(mouseHeading-2);
      }
      int rand_A = random(0, 2);
      maze.addwalls(mouseHeading);
      if (mouseHeading == 0) {
        if (rand_A == 0) {
          mouseColumn--;
          mouseHeading = 3;
        }
        else {
          mouseColumn++;
          mouseHeading = 1;
        }
      }
      else if (mouseHeading == 1) {
        if (rand_A == 0) {
          mouseRow--;
          mouseHeading = 0;
        }
        else {
          mouseRow++;
          mouseHeading = 2;
        }
      }
      else if (mouseHeading == 2) {
        if (rand_A == 0) {
          mouseColumn++;
          mouseHeading = 1;
        }
        else {
          mouseColumn--;
          mouseHeading = 3;
        }
      }
      else if (mouseHeading == 3) {
        if (rand_A == 0) {
          mouseRow++;
          mouseHeading = 2;
        }
        else {
          mouseRow--;
          mouseHeading = 0;
        }
      }
    }
    ////////////////////////////////////////////////////
    else if (condition == 2) {
      if(jun_cnt==1){
        maze.addMindwalls(mouseHeading-2);
      }
      int rand_A = random(0, 2);
      maze.addwalls(mouseHeading - 1);
      if (mouseHeading == 0) {
        if (rand_A == 0) {
          mouseRow--;
        }
        else {
          mouseColumn++;
          mouseHeading = 1;
        }
      }
      else if (mouseHeading == 1) {
        if (rand_A == 0) {
          mouseColumn++;
        }
        else {
          mouseRow++;
          mouseHeading = 2;
        }
      }
      else if (mouseHeading == 2) {
        if (rand_A == 0) {
          mouseRow++;
        }
        else {
          mouseColumn--;
          mouseHeading = 3;
        }
      }
      else if (mouseHeading == 3) {
        if (rand_A == 0) {
          mouseColumn--;
        }
        else {
          mouseRow--;
          mouseHeading = 0;
        }
      }
    }
    ////////////////////////////////////////////////////
    else if (condition == 3) {
      if(jun_cnt==1){
        jun_cnt = 0;
        maze.addMindwalls(mouseHeading-2);
      }
      maze.addwalls(mouseHeading);
      maze.addwalls(mouseHeading - 1);
      if (mouseHeading == 0) {
        mouseColumn++;
        mouseHeading = 1;
      }
      else if (mouseHeading == 1) {
        mouseRow++;
        mouseHeading = 2;
      }
      else if (mouseHeading == 2) {
        mouseColumn--;
        mouseHeading = 3;
      }
      else if (mouseHeading == 3) {
        mouseRow--;
        mouseHeading = 0;
      }
    }
    ///////////////////////////////////////////////////
    else if (condition == 4) {
      if(jun_cnt==1){
        maze.addMindwalls(mouseHeading-2);
      }
      int rand_A = random(0, 2);
      maze.addwalls(mouseHeading + 1);
      if (mouseHeading == 0) {
        if (rand_A == 0) {
          mouseColumn--;
          mouseHeading = 3;
        }
        else {
          mouseRow--;
        }
      }
      else if (mouseHeading == 1) {
        if (rand_A == 0) {
          mouseRow--;
          mouseHeading = 0;
        }
        else {
          mouseColumn++;
        }

      }
      else if (mouseHeading == 2) {
        if (rand_A == 0) {
          mouseColumn++;
          mouseHeading = 1;
        }
        else {
          mouseRow++;
        }
      }
      else if (mouseHeading == 3) {
        if (rand_A == 0) {
          mouseRow++;
          mouseHeading = 2;
        }
        else {
          mouseColumn--;
        }
      }
    }
    //////////////////////////////////////////////////
    else if (condition == 5) {
      if(jun_cnt==1){
        jun_cnt = 0;
        maze.addMindwalls(mouseHeading-2);
      }
      maze.addwalls(mouseHeading);
      maze.addwalls(mouseHeading + 1);
      if (mouseHeading == 0) {
        mouseColumn--;
        mouseHeading = 3;
      }
      else if (mouseHeading == 1) {
        mouseRow--;
        mouseHeading = 0;
      }
      else if (mouseHeading == 2) {
        mouseColumn++;
        mouseHeading = 1;
      }
      else if (mouseHeading == 3) {
        mouseRow++;
        mouseHeading = 2;
      }
    }
    //////////////////////////////////////////////////
    else if (condition == 6) {
      
      maze.addwalls(mouseHeading + 1);
      maze.addwalls(mouseHeading - 1);
      if (mouseHeading == 0) {
        mouseRow--;
      }
      else if (mouseHeading == 1) {
        mouseColumn++;
      }
      else if (mouseHeading == 2) {
        mouseRow++;
      }
      else if (mouseHeading == 3) {
        mouseColumn--;
      }
    }
    //////////////////////////////////////////////////
    else if (condition == 7) {
      maze.addwalls(mouseHeading - 1);
      maze.addwalls(mouseHeading);
      maze.addwalls(mouseHeading + 1);
      if (mouseHeading == 0) {
        mouseHeading = 2;
      }
      else if (mouseHeading == 1) {
        mouseHeading = 3;
      }
      else if (mouseHeading == 2) {
        mouseHeading = 0;
      }
      else if (mouseHeading == 3) {
        mouseHeading = 1;
      }
    }
    maze.print();
    delay(1000);
  }
  ///////////////////////////////////////////////////////////////////////////////////////////////end Heading to the target////////////////////////////////////////////////
  Serial.println("you have reached your target");
  ///////////////////////////////////////////////////////////////////////////////////////////////start Heading to the initial position////////////////////////////////////////////////
  while (mouseRow != init_mouseRow || mouseColumn != init_mouseColumn) {
    check();

    if (condition == 0) {
      if(jun_cnt==1){
        maze.addMindwalls(mouseHeading-2);
      }
      int rand_A = random(0, 3);
      if (mouseHeading == 0) {
        if (rand_A == 0) {
          mouseColumn--;
          mouseHeading = 3;
        }
        else if (rand_A == 1) {
          mouseRow--;
        }
        else if (rand_A == 2) {
          mouseColumn++;
          mouseHeading = 1;
        }
      }
      else if (mouseHeading == 1) {
        if (rand_A == 0) {
          mouseRow--;
          mouseHeading = 0;
        }
        else if (rand_A == 1) {
          mouseColumn++;
        }
        else if (rand_A == 2) {
          mouseRow++;
          mouseHeading = 2;

        }
      }
      else if (mouseHeading == 2) {
        if (rand_A == 0) {
          mouseColumn++;
          mouseHeading = 1;
        }
        else if (rand_A == 1) {
          mouseRow++;
        }
        else if (rand_A == 2) {
          mouseColumn--;
          mouseHeading = 3;
        }
      }
      else if (mouseHeading == 3) {
        if (rand_A == 0) {
          mouseRow++;
          mouseHeading = 2;
        }
        else if (rand_A == 1) {
          mouseColumn--;
        }
        else if (rand_A == 2) {
          mouseRow--;
          mouseHeading = 0;
        }
      }
    }
    //////////////////////////////////////////////////////
    else if (condition == 1) {
      if(jun_cnt==1){
        maze.addMindwalls(mouseHeading-2);
      }
      int rand_A = random(0, 2);
      maze.addwalls(mouseHeading);
      if (mouseHeading == 0) {
        if (rand_A == 0) {
          mouseColumn--;
          mouseHeading = 3;
        }
        else {
          mouseColumn++;
          mouseHeading = 1;
        }
      }
      else if (mouseHeading == 1) {
        if (rand_A == 0) {
          mouseRow--;
          mouseHeading = 0;
        }
        else {
          mouseRow++;
          mouseHeading = 2;
        }
      }
      else if (mouseHeading == 2) {
        if (rand_A == 0) {
          mouseColumn++;
          mouseHeading = 1;
        }
        else {
          mouseColumn--;
          mouseHeading = 3;
        }
      }
      else if (mouseHeading == 3) {
        if (rand_A == 0) {
          mouseRow++;
          mouseHeading = 2;
        }
        else {
          mouseRow--;
          mouseHeading = 0;
        }
      }
    }
    ////////////////////////////////////////////////////
    else if (condition == 2) {
      if(jun_cnt==1){
        maze.addMindwalls(mouseHeading-2);
      }
      int rand_A = random(0, 2);
      maze.addwalls(mouseHeading - 1);
      if (mouseHeading == 0) {
        if (rand_A == 0) {
          mouseRow--;
        }
        else {
          mouseColumn++;
          mouseHeading = 1;
        }
      }
      else if (mouseHeading == 1) {
        if (rand_A == 0) {
          mouseColumn++;
        }
        else {
          mouseRow++;
          mouseHeading = 2;
        }
      }
      else if (mouseHeading == 2) {
        if (rand_A == 0) {
          mouseRow++;
        }
        else {
          mouseColumn--;
          mouseHeading = 3;
        }
      }
      else if (mouseHeading == 3) {
        if (rand_A == 0) {
          mouseColumn--;
        }
        else {
          mouseRow--;
          mouseHeading = 0;
        }
      }
    }
    ////////////////////////////////////////////////////
    else if (condition == 3) {
      if(jun_cnt==1){
        jun_cnt = 0;
        maze.addMindwalls(mouseHeading-2);
      }
      maze.addwalls(mouseHeading);
      maze.addwalls(mouseHeading - 1);
      if (mouseHeading == 0) {
        mouseColumn++;
        mouseHeading = 1;
      }
      else if (mouseHeading == 1) {
        mouseRow++;
        mouseHeading = 2;
      }
      else if (mouseHeading == 2) {
        mouseColumn--;
        mouseHeading = 3;
      }
      else if (mouseHeading == 3) {
        mouseRow--;
        mouseHeading = 0;
      }
    }
    ///////////////////////////////////////////////////
    else if (condition == 4) {
      if(jun_cnt==1){
        maze.addMindwalls(mouseHeading-2);
      }
      int rand_A = random(0, 2);
      maze.addwalls(mouseHeading + 1);
      if (mouseHeading == 0) {
        if (rand_A == 0) {
          mouseColumn--;
          mouseHeading = 3;
        }
        else {
          mouseRow--;
        }
      }
      else if (mouseHeading == 1) {
        if (rand_A == 0) {
          mouseRow--;
          mouseHeading = 0;
        }
        else {
          mouseColumn++;
        }

      }
      else if (mouseHeading == 2) {
        if (rand_A == 0) {
          mouseColumn++;
          mouseHeading = 1;
        }
        else {
          mouseRow++;
        }
      }
      else if (mouseHeading == 3) {
        if (rand_A == 0) {
          mouseRow++;
          mouseHeading = 2;
        }
        else {
          mouseColumn--;
        }
      }
    }
    //////////////////////////////////////////////////
    else if (condition == 5) {
      if(jun_cnt==1){
        jun_cnt = 0;
        maze.addMindwalls(mouseHeading-2);
      }
      maze.addwalls(mouseHeading);
      maze.addwalls(mouseHeading + 1);
      if (mouseHeading == 0) {
        mouseColumn--;
        mouseHeading = 3;
      }
      else if (mouseHeading == 1) {
        mouseRow--;
        mouseHeading = 0;
      }
      else if (mouseHeading == 2) {
        mouseColumn++;
        mouseHeading = 1;
      }
      else if (mouseHeading == 3) {
        mouseRow++;
        mouseHeading = 2;
      }
    }
    //////////////////////////////////////////////////
    else if (condition == 6) {
      maze.addwalls(mouseHeading + 1);
      maze.addwalls(mouseHeading - 1);
      if (mouseHeading == 0) {
        mouseRow--;
      }
      else if (mouseHeading == 1) {
        mouseColumn++;
      }
      else if (mouseHeading == 2) {
        mouseRow++;
      }
      else if (mouseHeading == 3) {
        mouseColumn--;
      }
    }
    //////////////////////////////////////////////////
    else if (condition == 7) {
      maze.addwalls(mouseHeading - 1);
      maze.addwalls(mouseHeading);
      maze.addwalls(mouseHeading + 1);
      if (mouseHeading == 0) {
        mouseHeading = 2;
      }
      else if (mouseHeading == 1) {
        mouseHeading = 3;
      }
      else if (mouseHeading == 2) {
        mouseHeading = 0;
      }
      else if (mouseHeading == 3) {
        mouseHeading = 1;
      }
    }
    maze.print();
    delay(1000);
  }
  ///////////////////////////////////////////////////////////////////////////////////////////////end Heading to the initial position////////////////////////////////////////////////
  if (flag == 0) {
    maze.solve();
    flag++;
  }
  mouseHeading = init_mouseHeading;
  maze.print();
  delay(2000);
  ///////////////////////////////////////////////////////////////////////////////////////////////start following the shortest path////////////////////////////////////////////////
  while (mouseRow != maze.targetRow || mouseColumn != maze.targetColumn) {
    if (mouseHeading == 0) {
      if (maze.horizontalWalls[mouseRow][mouseColumn] == false && maze.values[mouseRow - 1][mouseColumn] == maze.values[mouseRow][mouseColumn] - 1) {
        mouseRow--;
      }
      else if (maze.verticalWalls[mouseRow][mouseColumn] == false && maze.values[mouseRow][mouseColumn - 1] == maze.values[mouseRow][mouseColumn] - 1) {
        mouseColumn--;
        mouseHeading = 3;
      }
      else if (maze.verticalWalls[mouseRow][mouseColumn + 1] == false && maze.values[mouseRow][mouseColumn + 1] == maze.values[mouseRow][mouseColumn] - 1) {
        mouseColumn++;
        mouseHeading = 1;
      }
      else {
        mouseRow++;
        mouseHeading = 2;
      }

    }
    else if (mouseHeading == 1) {
      if (maze.verticalWalls[mouseRow][mouseColumn + 1] == false && maze.values[mouseRow][mouseColumn + 1] == maze.values[mouseRow][mouseColumn] - 1) {
        mouseColumn++;

      }
      else if (maze.horizontalWalls[mouseRow][mouseColumn] == false && maze.values[mouseRow - 1][mouseColumn] == maze.values[mouseRow][mouseColumn] - 1) {
        mouseRow--;
        mouseHeading = 0;
      }
      else if (maze.horizontalWalls[mouseRow + 1][mouseColumn] == false && maze.values[mouseRow + 1][mouseColumn] == maze.values[mouseRow][mouseColumn] - 1) {
        mouseRow++;
        mouseHeading = 2;
      }
      else {
        mouseColumn--;
        mouseHeading = 3;
      }
    }
    else if (mouseHeading == 2) {
      if (maze.horizontalWalls[mouseRow + 1][mouseColumn] == false && maze.values[mouseRow + 1][mouseColumn] == maze.values[mouseRow][mouseColumn] - 1) {
        mouseRow++;
      }
      else if (maze.verticalWalls[mouseRow][mouseColumn + 1] == false && maze.values[mouseRow][mouseColumn + 1] == maze.values[mouseRow][mouseColumn] - 1) {
        mouseColumn++;
        mouseHeading = 1;
      }
      else if (maze.verticalWalls[mouseRow][mouseColumn] == false && maze.values[mouseRow][mouseColumn - 1] == maze.values[mouseRow][mouseColumn] - 1) {
        mouseColumn--;
        mouseHeading = 3;
      }
      else {
        mouseRow--;
        mouseHeading = 0;
      }
    }
    else if (mouseHeading == 3) {
      if (maze.verticalWalls[mouseRow][mouseColumn] == false && maze.values[mouseRow][mouseColumn - 1] == maze.values[mouseRow][mouseColumn] - 1) {
        mouseColumn--;

      }
      else if (maze.horizontalWalls[mouseRow + 1][mouseColumn] == false && maze.values[mouseRow + 1][mouseColumn] == maze.values[mouseRow][mouseColumn] - 1) {
        mouseRow++;
        mouseHeading = 2;
      }
      else if (maze.horizontalWalls[mouseRow][mouseColumn] == false && maze.values[mouseRow - 1][mouseColumn] == maze.values[mouseRow][mouseColumn] - 1) {
        mouseRow--;
        mouseHeading = 0;
      }
      else {
        mouseColumn++;
        mouseHeading = 1;
      }
    }
    maze.print();
    delay(2000);
  }
  ///////////////////////////////////////////////////////////////////////////////////////////////end following the shortest path////////////////////////////////////////////////
  while (flag == 1) {
    if (Serial.available()) {
      char push = Serial.read();
      if (push == 's') {
        Serial.println("End");
      }
    }
  }
  
}
