void check() {
  if (mouseHeading == 0) {
    if (analogRead(FNT) < fnt_tresh || maze.horizontalMindWalls[mouseRow][mouseColumn] == true/*when a wall apears infront*/) {
      fnt = 1;
    }
    else {
      fnt = 0;
    }

    if (analogRead(LFT) < lft_tresh || maze.verticalMindWalls[mouseRow][mouseColumn] == true/*when a wall apears on left*/) {
      lft = 2;
    }
    else {
      lft = 0;
    }
    if (analogRead(RGT) < rgt_tresh || maze.verticalMindWalls[mouseRow][mouseColumn + 1] == true/*when a wall apears on right*/) {
      rgt = 4;
    }
    else {
      rgt = 0;
    }
  }



  else if (mouseHeading == 1) {
    if (analogRead(FNT) < fnt_tresh || maze.verticalMindWalls[mouseRow][mouseColumn + 1] == true/*when a wall apears infront*/) {
      fnt = 1;
    }
    else {
      fnt = 0;
    }

    if (analogRead(LFT) < lft_tresh || maze.horizontalMindWalls[mouseRow][mouseColumn] == true/*when a wall apears on left*/) {
      lft = 2;
    }
    else {
      lft = 0;
    }
    if (analogRead(RGT) < rgt_tresh || maze.horizontalMindWalls[mouseRow + 1][mouseColumn] == true/*when a wall apears on right*/) {
      rgt = 4;
    }
    else {
      rgt = 0;
    }
  }



  else if (mouseHeading == 2) {
    if (analogRead(FNT) < fnt_tresh || maze.horizontalMindWalls[mouseRow + 1][mouseColumn] == true/*when a wall apears infront*/) {
      fnt = 1;
    }
    else {
      fnt = 0;
    }

    if (analogRead(LFT) < lft_tresh || maze.verticalMindWalls[mouseRow][mouseColumn + 1] == true/*when a wall apears on left*/) {
      lft = 2;
    }
    else {
      lft = 0;
    }
    if (analogRead(RGT) < rgt_tresh || maze.verticalMindWalls[mouseRow][mouseColumn] == true/*when a wall apears on right*/) {
      rgt = 4;
    }
    else {
      rgt = 0;
    }
  }



  else if (mouseHeading == 3) {
    if (analogRead(FNT) < fnt_tresh || maze.verticalMindWalls[mouseRow][mouseColumn] == true/*when a wall apears infront*/) {
      fnt = 1;
    }
    else {
      fnt = 0;
    }

    if (analogRead(LFT) < lft_tresh || maze.horizontalMindWalls[mouseRow + 1][mouseColumn] == true/*when a wall apears on left*/) {
      lft = 2;
    }
    else {
      lft = 0;
    }
    if (analogRead(RGT) < rgt_tresh || maze.horizontalMindWalls[mouseRow][mouseColumn] == true/*when a wall apears on right*/) {
      rgt = 4;
    }
    else {
      rgt = 0;
    }
  }
  condition = fnt + lft + rgt;
  ///////////////////////////////////////////////for simulating////////////////
  int b = 0;
  boolean sim_F = false;
  boolean sim_L = false;
  boolean sim_R = false;
  while (b != 1) {

    if (Serial.available()) {
      char a = Serial.read();
      if (a == '8') {
        sim_F = true;
      }
      else if (a == '4') {
        sim_L = true;
      }
      else if (a == '6') {
        sim_R = true;
      }
      else if (a == '1') {
        b = 1;
      }

    }
    else {
      continue;
    }
  }
  if (mouseHeading == 0) {
    if (sim_F == true || maze.horizontalMindWalls[mouseRow][mouseColumn] == true/*when a wall apears infront*/) {
      fnt = 1;
    }
    else {
      fnt = 0;
    }

    if (sim_L == true || maze.verticalMindWalls[mouseRow][mouseColumn] == true/*when a wall apears on left*/) {
      lft = 2;
    }
    else {
      lft = 0;
    }
    if (sim_R == true || maze.verticalMindWalls[mouseRow][mouseColumn + 1] == true/*when a wall apears on right*/) {
      rgt = 4;
    }
    else {
      rgt = 0;
    }
  }



  else if (mouseHeading == 1) {
    if (sim_F == true || maze.verticalMindWalls[mouseRow][mouseColumn + 1] == true/*when a wall apears infront*/) {
      fnt = 1;
    }
    else {
      fnt = 0;
    }

    if (sim_L == true || maze.horizontalMindWalls[mouseRow][mouseColumn] == true/*when a wall apears on left*/) {
      lft = 2;
    }
    else {
      lft = 0;
    }
    if (sim_R == true || maze.horizontalMindWalls[mouseRow + 1][mouseColumn] == true/*when a wall apears on right*/) {
      rgt = 4;
    }
    else {
      rgt = 0;
    }
  }



  else if (mouseHeading == 2) {
    if (sim_F == true || maze.horizontalMindWalls[mouseRow + 1][mouseColumn] == true/*when a wall apears infront*/) {
      fnt = 1;
    }
    else {
      fnt = 0;
    }

    if (sim_L == true || maze.verticalMindWalls[mouseRow][mouseColumn + 1] == true/*when a wall apears on left*/) {
      lft = 2;
    }
    else {
      lft = 0;
    }
    if (sim_R == true || maze.verticalMindWalls[mouseRow][mouseColumn] == true/*when a wall apears on right*/) {
      rgt = 4;
    }
    else {
      rgt = 0;
    }
  }



  else if (mouseHeading == 3) {
    if (sim_F == true || maze.verticalMindWalls[mouseRow][mouseColumn] == true/*when a wall apears infront*/) {
      fnt = 1;
    }
    else {
      fnt = 0;
    }

    if (sim_L == true || maze.horizontalMindWalls[mouseRow + 1][mouseColumn] == true/*when a wall apears on left*/) {
      lft = 2;
    }
    else {
      lft = 0;
    }
    if (sim_R == true || maze.horizontalMindWalls[mouseRow][mouseColumn] == true/*when a wall apears on right*/) {
      rgt = 4;
    }
    else {
      rgt = 0;
    }
  }
  condition = fnt + lft + rgt;
  /////////////////////////////////////////////////////////////////////////////
  int c = 0;
  int d = 0;
  if (mouseRow != init_mouseRow || mouseColumn != init_mouseColumn) {
    c = 1;
  }
  if (mouseRow != maze.targetRow || mouseColumn != maze.targetColumn) {
    d = 1;
  }

  if (condition == 7 && c == 1 && d == 1) {
    jun_cnt = 0;
  }
  else if (condition != 6) {
    jun_cnt++;
  }
}


void ir_or_enc_pid() {
  int kp = 1;
  if (analogRead(LFT) < lft_tresh) {
    //following the left wall
    L_err = analogRead(LFT) - initial_lft_val;
    L_diff = L_err * kp;
    mtr_cmd(L_base_speed - L_diff, R_base_speed + L_diff);
  }
  else if (analogRead(RGT) < rgt_tresh) {
    //following the right wall
    R_err = analogRead(RGT) - initial_rgt_val;
    R_diff = R_err * kp;
    mtr_cmd(L_base_speed - R_diff, R_base_speed + R_diff);
  }
  else {
    //encoder fwd with enc pid
  }
}

void cal() {
  for (int i = 0; i < 100; i++) {
    initial_lft_val = analogRead(LFT);
    initial_rgt_val = analogRead(RGT);

    L_total = L_total + initial_lft_val;
    R_total = R_total + initial_rgt_val;
  }

  initial_lft_val = L_total / 100;
  initial_rgt_val = R_total / 100;

  //turn left//

  /////////////
  R_total = 0;
  for (int i = 0; i < 100; i++) {
    initial_fnt_val = analogRead(FNT);

    F_total = F_total + initial_fnt_val;
    R_total = R_total + analogRead(RGT);
  }
  initial_fnt_val = F_total / 100;
  //turn right//

  /////////////

  //turn right//

  /////////////
  L_total = 0;
  for (int i = 0; i < 100; i++) {
    L_total = L_total + analogRead(LFT);
  }

  //turn left//

  /////////////
  F_total = 0;
  for (int i = 0; i < 100; i++) {
    F_total = F_total + analogRead(FNT);
  }

  lft_tresh = (initial_lft_val + L_total / 100) / 2;
  rgt_tresh = (initial_rgt_val + R_total / 100) / 2;
  fnt_tresh = (initial_fnt_val + F_total / 100) / 2;
}

