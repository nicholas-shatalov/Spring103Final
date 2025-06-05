#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>
AsyncDelay delay_win; // defining asyncdelay of 100ms
int randomarray[5] = {2,2,2,2,2};
int userinput[5] = {2,2,2,2,2};
int leaderboard[5] = {0,0,0,0,0};
int randnum;
bool state = true;
int leftbuttonpin = 4;
int rightbuttonpin = 5;
int switchpin = 7;
int score = 0;
int difficulty = 1000;
volatile bool leftflag = false;
volatile bool rightflag = false;
volatile bool switchflag = false;
volatile bool failcondition = false;
volatile bool printcondition = true;
int q=0;


void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);
  while(!Serial);
  pinMode(leftbuttonpin, INPUT_PULLDOWN);
  pinMode(rightbuttonpin, INPUT_PULLDOWN);
  pinMode(switchpin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(leftbuttonpin), lbutton, RISING);
  attachInterrupt(digitalPinToInterrupt(rightbuttonpin), rbutton, RISING);
  attachInterrupt(digitalPinToInterrupt(switchpin), iswitch, CHANGE);
  delay_win.start(100, AsyncDelay::MILLIS); // starts async read delay
}

void loop() {

  if(state) {
    CircuitPlayground.clearPixels();
    while(!leftflag && !rightflag) {
      if(printcondition) {
        difficulty = map(score, 0, 10, 1000, 50);
        if(difficulty<50) {
          difficulty = 50;
        }
        //Serial.println(difficulty); FOR DEBUG
        Serial.println("Welcome to the Tower Pattern Game!");
        Serial.println("");
        Serial.println("The objective of the game is to replicate the pattern flashed on the LEDs. Two yellow lights ");
        Serial.println("will light up inidicating that you can select either the left or the right button to light up.");
        Serial.println("Press the left button to light up a left light, and press the right button to light up a right light");
        Serial.println("Continue this process until you've replicated the pattern and you'll score a point!");
        Serial.println("But beware... as if you keep winning and scoring the patterns will flash for less time...");
        Serial.println("");
        Serial.println("Touch the A1 pin and press a button to begin!");
        Serial.print("Score: ");
        Serial.println(score);
        printcondition = false;
      }
      randomSeed(analogRead(1));
    }
    arrayreset();
    randompattern();
    tonepause();
    resetvalues();
  }

  else {
    buttonflags();
    if(q == 5) {
      winfailcheck();
      wincond();
      failcond();
    }
    else {
      ledchange();
    }
    // DEBUG PRINT STATEMENTS
    /*Serial.println(q);
    Serial.println(failcondition);
    Serial.print("User Array: ");
    for(int i=0; i<5; i++) {
      Serial.print(userinput[i]);
      Serial.print("  ");
    }
    Serial.print("Rand Array: ");
    for(int i=0; i<5; i++) {
      Serial.print(randomarray[i]);
      Serial.print("  ");
    }*/
    //Serial.println(state);
  }
}

void lbutton() {
  leftflag = true;
}

void rbutton() {
  rightflag = true;
}

void iswitch() {
  if(q == 5 && !state) {
    switchflag = true;
  }
}

void arrayreset() {
  for(int i=0; i<5; i++) {
    randomarray[i] = 2;
    userinput[i] = 2;
  }
}

void randompattern() {
  delay(1500);
  for(int i=0; i<5; i++){
    randnum = random(2);
    randomarray[i] = randnum;
    if(randomarray[i] == 0) {
      CircuitPlayground.setPixelColor(i, 255, 255, 255);
    }
    else {
      CircuitPlayground.setPixelColor(9-i, 255, 255, 255);
    }
  }
}

void tonepause() {
  CircuitPlayground.playTone(200, difficulty);
  CircuitPlayground.clearPixels();
}

void resetvalues() {
  q=0;
  state = false;
  leftflag = false;
  rightflag = false;
  failcondition = false;
}

void buttonflags() {
  if(leftflag) {
    delay(150);
    userinput[q] = 0;
    if(q<5) {
      q++;
    }
    leftflag = false;
  }
  else if(rightflag) {
    delay(150);
    userinput[q] = 1;
    if(q<5) {
      q++;
    }
    rightflag = false;
  }
}

void winfailcheck() {
  for(int i=0; i<5; i++) {
    if(userinput[i] != randomarray[i]) {
      failcondition = true;
      for(int o=0; o<10; o++) {
        CircuitPlayground.setPixelColor(o, 255, 0, 0);
      }
      break;
    }
  }
}

void wincond() {
  if(!failcondition) {
    for(int i=0; i<20; i++) {
      Serial.println("");
    }
    Serial.println("YOU WIN!!!!!!!!!!");
    for(int i=0; i<5; i++) {
      Serial.println("");
    }
    score++;
    while(!state) {
      leftflag = false;
      rightflag = false;
      winpattern();
      gamereset();
      CircuitPlayground.playTone(262.0, 15);
      CircuitPlayground.playTone(327.5, 15);
      CircuitPlayground.playTone(349.3, 15);
    }
  }
}

void winpattern() {
  for(int i=0; i<10; i++) {
    if(delay_win.isExpired()) {
      CircuitPlayground.clearPixels();
      // resets the previous lit up pixel

      int randsparkle = random(10);
      // generates random pixel number

      CircuitPlayground.setPixelColor(randsparkle, 255, 255, 255);
      // light up the random pixel white
      delay_win.repeat();
    }
  }
    // loop repeats the pattern for 10 different iterations
}

void failcond() {
  while(failcondition) {
    for(int i=0; i<20; i++) {
      Serial.println("");
    }
    Serial.println("You failed");
    Serial.print("Final Score: ");
    Serial.println(score);
    for(int i=0; i<5; i++) {
      /*if(score>leaderboard[i]) {
        if(i<4) {
          int j = 4-i;
          for(j>=0; j--;) {
            int k = i;
            leaderboard[k+1] = leaderboard[k];
            k++;
          }
        }
        leaderboard[i] = score;
        break;
      }*/
      Serial.println("");
    }
    /*Serial.print("Leaderboard: ");
    for(int y=0; y<5; y++) {
      Serial.print("#");
      Serial.println(y+1);
      Serial.print(" ");
      Serial.print(leaderboard[y]);
    }*/
    score = 0;
    while(!state) {
      leftflag = false;
      rightflag = false;
      CircuitPlayground.playTone(200, 2);
      gamereset();
    }
  }
}

void ledchange() {
  for(int j=0; j<5; j++) {
    if(userinput[j] == 0) {
      CircuitPlayground.setPixelColor(j, 255, 255, 255);
      CircuitPlayground.setPixelColor(9-j, 0, 0, 0);
    }
    else if(userinput[j] == 1) {
      CircuitPlayground.setPixelColor(9-j, 255, 255, 255);
      CircuitPlayground.setPixelColor(j, 0, 0, 0);
    }
    else {
      break;
    }
  }
  CircuitPlayground.setPixelColor(q, 255, 255, 0);
  CircuitPlayground.setPixelColor(9-q, 255, 255, 0);
}

void gamereset() {
  if(switchflag) {
    state = true;
    failcondition = false;
    switchflag = false;
    printcondition = true;
  }
}
