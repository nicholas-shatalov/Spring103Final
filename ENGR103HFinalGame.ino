#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>
AsyncDelay delay_win; // Async delay for winning light pattern

int randomarray[5] = {2,2,2,2,2}; // Arrays for generating random pattern and collecting user pattern
int userinput[5] = {2,2,2,2,2};
int q=0; // index for user inputs
int randnum; // random integer to determine either left or right LED
bool state = true; // to determine if the game is in idle state or play state
int leftbuttonpin = 4; // button and switch pins
int rightbuttonpin = 5;
int switchpin = 7;
int score = 0; // score counting integer
int difficulty = 1000; // delay time that starts at 1000ms and progressively gets smaller to increase difficulty
volatile bool leftflag = false; // interrupt flags
volatile bool rightflag = false;
volatile bool switchflag = false;
volatile bool failcondition = false; // condition that turns true if the user fails
volatile bool printcondition = true; // condition so that prints in idle state only occur once



void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);
  while(!Serial);
  pinMode(leftbuttonpin, INPUT_PULLDOWN); // initializes the buttons and switch
  pinMode(rightbuttonpin, INPUT_PULLDOWN);
  pinMode(switchpin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(leftbuttonpin), lbutton, RISING); // interrupts for buttons and switch
  attachInterrupt(digitalPinToInterrupt(rightbuttonpin), rbutton, RISING);
  attachInterrupt(digitalPinToInterrupt(switchpin), iswitch, CHANGE);
  delay_win.start(100, AsyncDelay::MILLIS); // starts async read delay
}

void loop() {
// if state is true run the pregame idle state
  if(state) {
    CircuitPlayground.clearPixels();
    while(!leftflag && !rightflag) {  // this runs only if no buttons have been pressed, allowing the user to exit the idle state by pressing a button
      if(printcondition) { // runs only if print condition is true so that prints in this idle state only occur when this condition is true

        difficulty = map(score, 0, 10, 1000, 25); // difficulty updates after each round - the map function maps the score to a delay time
        if(difficulty<25) { // boundary for map function
          difficulty = 25;
        }

        //Serial.println(difficulty); FOR DEBUG
        // Rules printed
        Serial.println("Welcome to the Tower Pattern Game!");
        Serial.println("Hold the CPX board with the USB at the bottom.");
        Serial.println("");
        Serial.println("The objective of the game is to replicate the pattern flashed on the LEDs. Replicate the white");
        Serial.println("pattern that flashes with a beep! Two yellow lights will light up as a preview");
        Serial.println("to indicate that you can select either the left or the right button to light up.");
        Serial.println("Press the left button to light up a left light, and press the right button to light up a right light.");
        Serial.println("Continue this process until you've replicated the pattern and you'll score a point!");
        Serial.println("But beware... as if you keep winning and scoring the patterns will flash for less time...");
        Serial.println("");
        Serial.println("Touch the A1 pin and press a button to begin!");
        Serial.print("Score: ");
        Serial.println(score);
        printcondition = false; // Sets print condition to false so this only occurs once
      }
      randomSeed(analogRead(1)); // random seed on pin A1 in order to ensure random patterns
    }
    // after idle while loop is exited, initial pregame setup code runs
    arrayreset(); // reset function that resets the random and user input arrays after each round
    randompattern(); // random pattern function which generates a random LED pattern and displays it
    tonepause(); // a tone plays to alert the user of the random pattern, tone only plays for the length of the difficulty delay time
    resetvalues(); // button and switch flags, fail conditions, button press index, and idle state get reset and set to false to start the gameplay
  }

  else { // runs when the idle state is false to begin the game start
    buttonflags(); // function that increments an index and records user input choice on button press
    if(q == 5) { // if the user has made a choice for all 5 leds run this section
      winfailcheck(); // compare the random array and user input array to check if they match, if they don't match set the fail condition to true and turn LEDs red
      wincond(); // only run this code if the fail condition is false
      failcond(); // run this code if the fail condition is true
      // the fail and win conditions will continue running until the user flicks the switch to reset the game
    }
    else { // if the user has made a choice for a left or right button, and not all choices have been made yet
      ledchange(); // light up the LEDs that the user has chosen
    }
    // the code will continue repeating until the user has made all choices
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

void lbutton() { // left button interrupt flags
  leftflag = true;
}

void rbutton() { // right button interrupt flags
  rightflag = true;
}

void iswitch() { // switch interrupt flags
  if(q == 5 && !state) { // only set the switch flag to true if the game is in play mode and the user has finished all their turns
    switchflag = true;
  }
}

void arrayreset() { // resets all array values to 2
  for(int i=0; i<5; i++) {
    randomarray[i] = 2;
    userinput[i] = 2;
  }
}

void randompattern() {
  delay(1500); // wait time after the user presses a button in order to allow user to remove their thumbs off the board so they don't block the light pattern
  for(int i=0; i<5; i++){ // generates a random array of 5 values that are either 0 or 1, then lights up either a left or right LED corresponding to 0 or 1
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

void tonepause() { // pause to allow user to see the pattern, length of pause depends on difficulty
  CircuitPlayground.playTone(200, difficulty);
  CircuitPlayground.clearPixels();
}

void resetvalues() { // reset function to restore everything back to original idle state
  q=0;
  state = false;
  leftflag = false;
  rightflag = false;
  failcondition = false;
}

void buttonflags() { // if the user presses the left button, add a 0 to the array, if the user presses the right button, add a 1 to the array
  if(leftflag) {
    delay(150); // debounce time
    userinput[q] = 0;
    if(q<5) {
      q++;
    }
    leftflag = false;
  }
  else if(rightflag) {
    delay(150); // debounce time
    userinput[q] = 1;
    if(q<5) {
      q++;
    }
    rightflag = false;
  }
}

void winfailcheck() { // compares the two arrays, and if they don't match, set the fail condition to true and light the LEDs up red. if they do match move onto wincond function
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

void wincond() { // if the fail condition isn't true, print text on the serial monitor stating you win, increment the score, play the win pattern with nice noises, 
                 // and continue checking if the user resets the game
  if(!failcondition) {
    for(int i=0; i<20; i++) {
      Serial.println("");
    }
    Serial.println("YOU WIN!!!!!!!!!!");
    for(int i=0; i<5; i++) {
      Serial.println("");
    }
    score++;
    while(!state) { // while the game isn't reset back to idle state
      leftflag = false; // continuously sets button flags false so the user doesn't skip over the idle state on accident
      rightflag = false;
      winpattern(); // LED pattern for win
      gamereset(); // will continue checking if the user sets the switch flag to true, then the game will reset
      CircuitPlayground.playTone(262.0, 15); // notes of C-major chord that play very briefly
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
  while(failcondition) { // runs while fail condition is true
    for(int i=0; i<20; i++) { // prints you failed text and the final score
      Serial.println("");
    }
    Serial.println("You failed");
    Serial.print("Final Score: ");
    Serial.println(score);
    for(int i=0; i<5; i++) {
      Serial.println("");
    }
    score = 0; // resets the score to 0
    while(!state) { // while the user hasn't reset the game
      leftflag = false; // continuously sets button flags false so the user doesn't skip over the idle state on accident
      rightflag = false;
      CircuitPlayground.playTone(200, 2); 
      gamereset(); // continuously checks if the user resets the game 
    }
  }
}

void ledchange() { // changes the LEDs lit up during play based on user input
  for(int j=0; j<5; j++) {
    if(userinput[j] == 0) { // lights up the left LED if the user input is 0 
      CircuitPlayground.setPixelColor(j, 255, 255, 255);
      CircuitPlayground.setPixelColor(9-j, 0, 0, 0);
    }
    else if(userinput[j] == 1) { // lights up the right LED if the user input is 1
      CircuitPlayground.setPixelColor(9-j, 255, 255, 255);
      CircuitPlayground.setPixelColor(j, 0, 0, 0);
    }
    else { // if the user has not chosen every LED yet, then break the loop after all previously chosen LEDs have been lit up
      break;
    }
  }
  CircuitPlayground.setPixelColor(q, 255, 255, 0); // yellow preview pixels
  CircuitPlayground.setPixelColor(9-q, 255, 255, 0);
}

void gamereset() { // runs if the user flicks the switch after the game is over
  if(switchflag) { 
    state = true; // resets all values to return to idle game state
    failcondition = false;
    switchflag = false;
    printcondition = true;
  }
}
