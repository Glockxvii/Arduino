#include <LedControl.h>
#include <LiquidCrystal.h>

// Matrix LED Display pin numbers
const int DIN_PIN = A2;
const int CLK_PIN = A3;
const int CS_PIN = A4;
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);
// LCD 2x16 Display setup
LiquidCrystal lcd(12,11,5,4,3,2);
// Joystick pin numbers
const int SW_pin = A5; // analog pin connected to switch output
const int X_pin = A0; // analog pin connected to X output
const int Y_pin = A1; // analog pin connected to Y output

void setup() {
  // Joystick initialization
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  //LED matrix initialization
  lc.shutdown(0, false);
  lc.setIntensity(0,0);
  lc.clearDisplay(0);
  // LCD display initialization
  lcd.begin(16,2);
  //Print
  Serial.begin(9600);
  
}

void matrixDot(){

  //X 0-450 go left
  //X 560 to 1023 go right
  //Y 0 to 450 go up
  //Y 560 to 1023 go down

  static int dotRow=3; // Left to Right
  static int dotCol=3; // Down to up
  
  int xAxis = analogRead(X_pin); // Current X
  int yAxis = analogRead(Y_pin); // Current Y

  // Timing
  static unsigned long lastMoveTime = 0;
  const unsigned long moveDelay = 150;
  unsigned long currentTime = millis();

  if(currentTime - lastMoveTime >= moveDelay){

    lc.setLed(0,dotRow,dotCol,false);

    if (xAxis <450 && dotRow > 0){ // Move Left
      dotRow--;
      Serial.print("move Left");
      
    }

    if (xAxis > 560 && dotRow < 7){ // Move Right
      dotRow++;
      Serial.print("Move Right");
    }

    if(yAxis < 450 && dotCol < 7){ // Move up
      dotCol++;
      Serial.print("Move Up");
    }
    if(yAxis > 560 && dotCol > 0){ // Move down
      dotCol--;
      Serial.print("Move Down");
    }
    lastMoveTime = currentTime;
    lc.setLed(0,dotRow,dotCol,true);
  }
}

void loop() {
  static unsigned long lastUpdate = 0;
  const unsigned long updateDelay = 1000;
  unsigned long currentUpdate = millis();
  if(currentUpdate - lastUpdate >= updateDelay){
    Serial.print("Switch:  ");
    Serial.print(digitalRead(SW_pin));
    Serial.print("\n");
    Serial.print("X-axis: ");
    Serial.print(analogRead(X_pin));
    Serial.print("\n");
    Serial.print("Y-axis: ");
    Serial.println(analogRead(Y_pin));
    Serial.print("\n\n");
    lastUpdate=currentUpdate;
  }

  matrixDot();
  
  
}
