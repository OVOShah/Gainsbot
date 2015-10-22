
#include <Servo.h> 

Servo myservo1;
Servo myservo2;
Servo myservo3; 
Servo myservo4;

int servoVal;

const int baseY = 0;
const int baseX = 1;
const int armY = 2;
const int armX = 3;

int curBaseY = 90;
int curBaseX = 90;
int curArmY = 90;
int curArmX = 90;

int baseXMotion[150];
int baseYMotion[150];
int armXMotion[150];
int armYMotion[150];

int counter = 0;
int playCounter = 0;
int ledCounter = 0;

const int motionButton = 7;
const int ledRed = 8;
const int ledGreen = 12;

int buttonState = 0;
boolean storingMotions = false;
boolean playingMotions = false;

void setup() {
  myservo1.attach(9);
  myservo2.attach(10); 
  myservo3.attach(11);
  myservo4.attach(6);
  
  //Move servo to neutral position
  myservo4.write(90);
  myservo2.write(90);
  myservo3.write(90);
  myservo1.write(90);
  
  //Initiate inputs and outputs
  pinMode(motionButton,INPUT);
  pinMode(ledRed,OUTPUT);
  pinMode(ledGreen,OUTPUT);
  digitalWrite(ledGreen,HIGH);
}

void loop() {
  
  //Move servos based on 4 inputs by joysticks
  if (!playingMotions){
    curBaseX = moveServo(baseX, myservo4, curBaseX);
    curBaseY = moveServo(baseY, myservo2, curBaseY);
    curArmY = moveServo(armY, myservo3, curArmY);
    curArmX = moveServo(armX, myservo1, curArmX);
  }
  
  buttonState = digitalRead(motionButton);
  
  //Cancel pressed
  if (buttonState == HIGH && (playingMotions || storingMotions)){
    storingMotions = false;
    playingMotions = false;
    digitalWrite(ledGreen,HIGH);
    digitalWrite(ledRed,LOW);
    Serial.println("Cancel pressed");
    delay(400);
  }
  
  //Store motions pressed
  else if (buttonState == HIGH && !playingMotions) {
    counter = 0;
    storingMotions = true;]
    delay(200);
  }
  
  //Currently storing motions, flash LED and store joystick values in arrays
  else if (storingMotions){
    ledCounter++;
    digitalWrite(ledGreen,LOW);
    
    playingMotions = storeMotions();
    delay(25);
    
    if (ledCounter <= 5){
      digitalWrite(ledRed,HIGH);
    }
    else if (ledCounter == 10){
      ledCounter = 0;
    }
    else if (ledCounter > 5){
      digitalWrite(ledRed,LOW);
    }
    
  }
  
  //Replaying stored motions
  else if (playingMotions){
    digitalWrite(ledRed,HIGH);
    digitalWrite(ledGreen,LOW);
    
    myservo4.write(baseXMotion[playCounter]);
    myservo2.write(baseYMotion[playCounter]);
    myservo3.write(armYMotion[playCounter]);
    myservo1.write(armXMotion[playCounter]);
    
    delay(30);
    
    //Loop stored motion
    if (playCounter == 150){
      playCounter = 1;
    }
    else {
      playCounter++;
    }
  }
  
}

//Method used to move servo based on analog input, returns servo location incase motion is being stored
int moveServo(int analogIn, Servo servo, int curVal) {
  servoVal = analogRead(analogIn);
  servoVal = map(servoVal, 0, 1023, -3, 3);
  
  delay(5);
  
  if (curVal > 180){curVal = 180;}
  else if (curVal < 0){curVal = 0;}
  else{curVal = curVal + servoVal;}
  
  servo.write(curVal);
  return curVal;
      
}

//Method to store each servos motions, returns false if arrays are full
boolean storeMotions(){
  
  baseXMotion[counter] = curBaseX;
  baseYMotion[counter] = curBaseY;
  armXMotion[counter] = curArmX;
  armYMotion[counter] = curArmY;
  
  if (counter == 150){
    storingMotions = false;
    playCounter = 1;
    Serial.println("End of store memory");
    return true;
  }
  
  else{
    counter++;
    return false;
  }
}



