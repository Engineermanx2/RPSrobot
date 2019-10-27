#include <Arduino.h>
//-------------------------------------------------------------------------------------------------------------------
//gamelogic setup
bool Pregame = true, Ingame = false; 
int readystate, startState, up_count = 0, down_count = 0, ROUND = 1, person_score = 0, robot_score = 0, play = 0;
char gameDif, updown;

//-------------------------------------------------------------------------------------------------------------------
// box setup

//Introduce LCD Library and call out pins for LCD, LEDS and Stepper Motor

#include <LiquidCrystal.h>
#include <Wire.h>
LiquidCrystal lcd(35,37,39,41,43,45);

#define led1 49
#define led2 51
#define led3 53
#define buzz 2 

#define dirPin 30
#define stepPin 28
#define stepsPerRev 236


/*bool start = false; 
bool easy = false;
bool medium = false;
bool hard = false; */

//--------------------------------------------------------------------------------------------------------------------
//hand arm setup
#include <Servo.h>

const int p = 11, psense = 22, r = 10, rsense = 23, m = 9, msense = 24, i = 8, isense = 25, t = 12, tsense = 26, tr = 13, cw = 10, ccw = 170, hold = 90;
int pval = HIGH, rval = HIGH, mval = HIGH, ival = HIGH, tval = HIGH;

Servo pinky;
Servo ring;
Servo middle;
Servo index;
Servo thumb;
Servo thumbrot;


//--------------------------------------------------------------------------------------------------------------------
//Serial communicaiton values 
#define start 1
#define ready 2
#define up 3
#define down 4
#define PlayerRock 5
#define PlayerPaper 6
#define PlayerScissor 7
char reset = 8; 

//--------------------------------------------------------------------------------------------------------------------------------
//box functions
void led_off(){
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW); 
}

void LED_blink(){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    delay(500); 
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    delay(500);
    
}

void UP(){
  //Larger number reduces speed and vice versa 
  int stepSpeed = 1300; 
   //Set motor direction clockwise
  digitalWrite(dirPin, HIGH); 

  //For 90 degree turn or 1/4 a revolution
  for (int i = 0; i < stepsPerRev/4; i++)
  {
     digitalWrite(stepPin, HIGH);  //These four lines result in 1 step
     delayMicroseconds(stepSpeed); 
     digitalWrite(stepPin, LOW);
     delayMicroseconds(stepSpeed); 
   
  }
}

void DOWN(){
  int stepSpeed = 1300;
  //Set motor direction counter-clockwise 
  digitalWrite(dirPin, LOW); 

  for(int i = 0; i < stepsPerRev/4; i++)
  {
     digitalWrite(stepPin, HIGH);
     delayMicroseconds(stepSpeed);
     digitalWrite(stepPin, LOW);
     delayMicroseconds(stepSpeed); 
  }
}

//--------------------------------------------------------------------------------------------------------------------------------
//servo functions

void score(){
  for(int j = 0; j<= 100; j++){
      if (Serial.available()>0){
        int person_play = (Serial.read()- '0');
        if (((person_play == PlayerRock) && (play == 1)) || ((person_play == PlayerPaper) && (play == 2)) || ((person_play == PlayerScissor) && (play == 3))){
          lcd.clear();
          lcd.write("      TIE!      ");
          j = 101;
          
          delay(1000);
          
        }
        else if((person_play == PlayerRock && play == 2) || (person_play == PlayerPaper && play == 3) || (person_play == PlayerScissor && play == 1)){
          //robot win
          lcd.clear();
          lcd.write("   Robot Win   ");
          robot_score++;
          
          j = 101;
          delay(1000);

        }
        else if(((person_play == PlayerPaper) && (play == 1)) ||((person_play == PlayerScissor) && (play == 2)) || ((person_play == PlayerRock) && (play == 3))){
          lcd.clear();
          lcd.write("   Person Win   ");
          person_score++;
          
          j = 101;
          delay(1000);
        }
        lcd.clear();
        lcd.write("YOU   game   BOT"); 
        lcd.setCursor(0,1);
        lcd.write(" ");
        lcd.print(person_score);
        lcd.write("     ");
        lcd.print(ROUND);
        lcd.write("      "); 
        lcd.print(robot_score);
      }
      delay(5);
  }
}

void HOLD(){    //hold servos current position
  pinky.write(hold);
  ring.write(hold);
  middle.write(hold);
  index.write(hold);
  thumb.write(hold);
  delay(200);
  return;
}

void stopservo(){       //stop all servo movement
  
  for(int i = 0; i <= 60; i++){
   pval = digitalRead(psense);
   rval = digitalRead(rsense);
   mval = digitalRead(msense);
   ival = digitalRead(isense);
   tval = digitalRead(tsense);
   
   if( pval == LOW){
    pinky.write(hold);
   }
   if( rval == LOW){
    ring.write(hold);
   }
   if( mval == LOW){
    middle.write(hold);
   }
   if( ival == LOW){
    index.write(hold);
   }
   if( tval == LOW){
    thumb.write(hold);
   }
   if(pval == LOW && rval == LOW && mval == LOW && ival == LOW && tval == LOW){
    i = 70;
   }
   delay(10);
  }
  HOLD();
  return;
}

void scissors(){    //scissors subfuction (fist to scissors)
   //open hand to scissors
  pinky.write(hold);
  ring.write(hold);
  middle.write(cw);
  index.write(ccw);
  thumb.write(hold);
  thumbrot.write(15);
  delay(200);

  //stop the moving fingers
  for(int i = 0; i <= 60; i++){
   mval = digitalRead(msense);
   ival = digitalRead(isense);

   if( mval == LOW){
    middle.write(hold);
   }
   if( ival == LOW){
    index.write(hold);
   }
   if(mval == LOW && ival == LOW ){
    i = 70;
   }
   delay(10);
  }
  HOLD();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write("Scissors");
  delay(500);
  score();
  delay(2500);
  
  //close fingers to rock
  pinky.write(hold);
  ring.write(hold);
  middle.write(ccw);
  index.write(cw);
  thumb.write(hold);
  thumbrot.write(15);
  delay(200);

  //stop moving fingers

 for(int i = 0; i <= 60; i++){
   mval = digitalRead(msense);
   ival = digitalRead(isense);

   if( mval == LOW){
    middle.write(hold);
   }
   if( ival == LOW){
    index.write(hold);
   }
   if(mval == LOW && ival == LOW ){
    i = 70;
   }
   delay(10);
  }
  HOLD();
  return;
  //end of scissors
}

void paper(){     //paper subfunction (fist to paper)
  //open hand to paper
  pinky.write(ccw);
  ring.write(cw);
  middle.write(cw);
  index.write(ccw);
  thumb.write(ccw);
  thumbrot.write(88);
  delay(200);

  //stop finger movement
  for(int i = 0; i <= 60; i++){
   pval = digitalRead(psense);
   rval = digitalRead(rsense);
   mval = digitalRead(msense);
   ival = digitalRead(isense);
   tval = digitalRead(tsense);

   if( pval == LOW){
    pinky.write(hold);
   }
   if( rval == LOW){
    ring.write(hold);
   }
   if( mval == LOW){
    middle.write(hold);
   }
   if( ival == LOW){
    index.write(hold);
   }
   if( tval == LOW){
    thumb.write(hold);
   }
   if(pval == LOW && rval == LOW && mval == LOW && ival == LOW && tval == LOW){
    i = 70;
   }
   delay(10);
  }
  HOLD();
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write("Paper");
  delay(500);
  score();
  delay (2500);

  //close hand to fist
  pinky.write(cw);
  ring.write(ccw);
  middle.write(ccw);
  index.write(cw);
  thumb.write(cw);
  thumbrot.write(15);
  delay(200);

  //stop finger movement
  for(int i = 0; i <= 60; i++){
   pval = digitalRead(psense);
   rval = digitalRead(rsense);
   mval = digitalRead(msense);
   ival = digitalRead(isense);
   tval = digitalRead(tsense);

   if( pval == LOW){
    pinky.write(hold);
   }
   if( rval == LOW){
    ring.write(hold);
   }
   if( mval == LOW){
    middle.write(hold);
   }
   if( ival == LOW){
    index.write(hold);
   }
   if( tval == LOW){
    thumb.write(hold);
   }
   if(pval == LOW && rval == LOW && mval == LOW && ival == LOW && tval == LOW){
    i = 70;
   }
   delay(10);
  }
  HOLD();
  return;
}

void rock(){      //fist to rock (nothing needed
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write("Rock");
  delay(500);
  score();
  delay(2500);
  return;
}

void readyup(){    // close hand to fist for ready up
  //close hand to fist
  pinky.write(cw);
  ring.write(ccw);
  middle.write(ccw);
  index.write(cw);
  thumb.write(cw);
  thumbrot.write(15);
  delay(200);

  //stop finger movement
  for(int i = 0; i <= 60; i++){
   pval = digitalRead(psense);
   rval = digitalRead(rsense);
   mval = digitalRead(msense);
   ival = digitalRead(isense);
   tval = digitalRead(tsense);

   if( pval == LOW){
    pinky.write(hold);
   }
   if( rval == LOW){
    ring.write(hold);
   }
   if( mval == LOW){
    middle.write(hold);
   }
   if( ival == LOW){
    index.write(hold);
   }
   if( tval == LOW){
    thumb.write(hold);
   }
   if(pval == LOW && rval == LOW && mval == LOW && ival == LOW && tval == LOW){
    i = 70;
   }
   delay(10);
  }
  HOLD();
  return;
}

void endgame(){     //signal end of game with open hand
  lcd.clear();
  if(person_score > robot_score){
   lcd.write("YOU");
    lcd.setCursor(0,1);
    lcd.write("WIN");
  }
  else if(person_score < robot_score){
    lcd.write("ROBOT");
    lcd.setCursor(0,1);
    lcd.write("WIN");
  }
  else if(person_score == robot_score){
    lcd.write("TIE");
  }

  //open hand to paper
  pinky.write(ccw);
  ring.write(cw);
  middle.write(cw);
  index.write(ccw);
  thumb.write(ccw);
  thumbrot.write(88);
  Serial.write(reset);
  delay(200);
  


  //stop finger movement
  for(int i = 0; i <= 60; i++){
   pval = digitalRead(psense);
   rval = digitalRead(rsense);
   mval = digitalRead(msense);
   ival = digitalRead(isense);
   tval = digitalRead(tsense);

   if( pval == LOW){
    pinky.write(hold);
   }
   if( rval == LOW){
    ring.write(hold);
   }
   if( mval == LOW){
    middle.write(hold);
   }
   if( ival == LOW){
    index.write(hold);
   }
   if( tval == LOW){
    thumb.write(hold);
   }
   if(pval == LOW && rval == LOW && mval == LOW && ival == LOW && tval == LOW){
    i = 70;
   }
   delay(10);
  }
  HOLD();
  delay(1000);
  return;
}

void move(int play_move){
  if (play_move == 1){
    rock ();
  }
  else if(play_move == 2){
    paper();
  }
  else if(play_move == 3){
    scissors();
  }
  else{

  }
}

void setup() {

    Serial.begin(9600);

  //box setup-------------------------------------------------

    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);

    lcd.begin(16,2);
    lcd.clear();

    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(buzz, OUTPUT);  

    lcd.write("Press"); 
    lcd.setCursor(0,1);
    lcd.write("Start"); 

    led_off(); 
  
  //hand setup--------------------------------------------- 
  pinMode(psense, INPUT);
  pinMode(rsense, INPUT);
  pinMode(msense, INPUT);
  pinMode(isense, INPUT);
  pinMode(52, OUTPUT);
  pinky.attach(p, 900, 2100);
  ring.attach(r, 900, 2100);
  middle.attach(m, 900, 2100);
  index.attach(i, 900, 2100);
  thumb.attach(t, 900, 2100);
  thumbrot.attach(tr, 500, 2100);
  thumbrot.write(88);
}


//--------------------------------------------------------------------------------------------------------------------------------
//loop
void loop(){
while (Pregame && !Ingame){
  lcd.clear();
  lcd.write("Press"); 
  lcd.setCursor(0,1);
  lcd.write("Start"); 
  LED_blink();
  /*if(Serial.available()>0){
    gameDif = Serial.read();//change to start
    if(gameDif == "hard" || gameDif == "medium" || gameDif == "easy"){
      Pregame = FALSE;
      Ingame = TRUE;
    }}*/
  
 
 
 
 
 
  if(Serial.available()>0){
      startState = (Serial.read() - '0');
      if(startState == start) {
          Pregame = false; 
          Ingame = true;
          ROUND = 0; 
          person_score = 0;
          robot_score = 0;
      }
  }
}

  while (!Pregame && Ingame){
    lcd.clear();
    while(ROUND <3){
      
      if(Serial.available()>0){
         readystate = (Serial.read() - '0');
         if(readystate == ready){
           readyup();
         while (readystate == ready){ 
          delay(200);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.write("Ready!");
          led_off();
          
         if (Serial.available()>0){
            updown = (Serial.read() - '0');
           if(updown == up){
              //stepper up command
              digitalWrite(buzz, LOW);
             up_count++;

           }
           else if(updown == down){
              //stepper down command
              digitalWrite(buzz, HIGH);
              down_count++;
            }
            if(up_count == 1 && down_count == 1){
              digitalWrite(led3, HIGH);
              delay(2);
            }
            else if(up_count == 2 && down_count == 2){
              digitalWrite(led3, HIGH);
              digitalWrite(led2, HIGH);
              delay(2);
            }
            if(up_count == 3 && down_count == 3){
              digitalWrite(led3, HIGH);
              digitalWrite(led2, HIGH);
              digitalWrite(led1, HIGH);
              digitalWrite(buzz, HIGH); 
              play = random(1, 4); // randomly choose what to play with RNG
              ROUND++;
              digitalWrite(buzz, LOW);
              move(play);   //plays the corresponding rock paper or scissors
              up_count = 0;
              down_count = 0;
              if(ROUND == 3){
                readystate = 0;
                break;
              }
           }
           else{
            
            }
          }
        }
      }
      else{
        delay(5);
      }
      }
      else{
       delay(5);
       
 
      }
    delay(5);
    }
  Pregame = true;
  Ingame = false;
  delay(500);
  endgame();

  }

  
}


