
#include "Arduino.h";
#include "SoftwareSerial.h";
#include "DFRobotDFPlayerMini.h";

SoftwareSerial mySoftwareSerial(10,11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

int testButtonPin = 7;
int ledPin        = 13;

void setup() {

  
        digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(500);              // wait for a second
        digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
        delay(1000);  
        digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(500);              // wait for a second
        digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
        delay(1000);  
        digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(500);              // wait for a second
        digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
        delay(1000);  
        
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);  
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    printDetail(myDFPlayer.readType(), myDFPlayer.read());
    while(true) {
        digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(300);              // wait for a second
        digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
        delay(300);       
    }
  }
  Serial.println(F("DFPlayer Mini online."));  
  
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  myDFPlayer.play(2);
  
  pinMode(testButtonPin,  INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);   
}

void loop() {

  // read the state of the pushbutton value:
  int buttonState = digitalRead(testButtonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == LOW) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    myDFPlayer.play(3);
    digitalWrite(ledPin, LOW);
    
  }
  
  //Print the detail message from DFPlayer to handle different errors and states.
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read());
  }
  
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}


