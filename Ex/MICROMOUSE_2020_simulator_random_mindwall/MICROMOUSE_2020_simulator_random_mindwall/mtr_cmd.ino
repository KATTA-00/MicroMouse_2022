
void mtr_cmd(int lm, int rm) {
  /*/////////lm///////////////////////
  if (0 < lm) {
    if (lm >= 255) {
      lm = 255;
    }
    else {
      lm = lm;
    }

  }


  else {
    if (lm <= -255) {
      lm = -255;
    }
    else {
      lm = lm;
    }
  }

  ///////////rm/////////////////////
  if (0 < rm) {
    if (rm >= 255) {
      rm = 255;
    }
    else {
      rm = rm;
    }
  }


  else {
    if (rm <= -255) {
      rm = -255;
    }
    else {
      rm = rm;
    }
  }*/

  lm=constrain(lm,-255,255);
  rm=constrain(rm,-255,255);

  ///////////////////Lmtr//////////////

  if (lm > 0) {

    digitalWrite(leftF, HIGH);
    digitalWrite(leftB, LOW);
    analogWrite(LM, lm);
  }

  else {
    digitalWrite(leftF, LOW);
    digitalWrite(leftB, HIGH);
    analogWrite(LM, (-1)*lm);
  }

  /////////////////////Rmtr////////////////
  if (rm > 0) {
    digitalWrite(rightF, HIGH);
    digitalWrite(rightB, LOW);
    analogWrite(RM, rm);
  }

  else {
    digitalWrite(rightF, LOW);
    digitalWrite(rightB, HIGH);
    analogWrite(RM, (-1)*rm);
  }
}

void brk(){
  digitalWrite(leftF,HIGH);
  digitalWrite(leftB,HIGH);
  digitalWrite(rightF,HIGH);
  digitalWrite(rightB,HIGH);
  
}
