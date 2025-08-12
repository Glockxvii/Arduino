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
// Button
const int Button_pin = 7; // button pin
const int Delete_button_pin = 6; //delete button
static bool isDeleting = false; // Is delete button active or not
static bool confirmDelete = false; // Confirm delete painting

// Matrix Led variables
static int dotRow=3; // Left to Right
static int dotCol=3; // Down to up
// Matrix Led Storage Array
const int maxDots = 64;
static int dotRowArray[maxDots]; //Returned dotRowArray[paintedIndex] when a button is pressed.
static int dotColArray[maxDots];
static int paintedIndex = 0; // An index of dot locations
static bool LCDinfo = true;


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
  // Button initialization
  pinMode(Button_pin, INPUT_PULLUP);
  pinMode(Delete_button_pin, INPUT_PULLUP);
  //Print
  Serial.begin(9600);
  
}
void deleteButton(){
  static int lastTap =0;
  int currentTap = digitalRead(Delete_button_pin);
  static unsigned long lastMillis;
  unsigned long currentMillis=millis();
  static unsigned long lastMillisNo;
  unsigned long currentMillisNo=millis();
  static int printStep = 0;
  static int printStepNO = 0; // Steps to printing Y E S
  static bool joystickSelectYes = false; // Select if true or false when chosing YES
  static bool joystickSelectNo = false;
  int Xvalue = analogRead(X_pin);
  static bool showedYES = false; // Has the full YES been shown
  static bool showedNO = false;
  int currentState = digitalRead(SW_pin); //Joystick states
  static int beforeState; // Joystick states
  static bool hasClicked = false;

  //Joystic click confirm
  if(currentState!=beforeState){
    if(beforeState==HIGH){
      hasClicked=!hasClicked;
    }
    
    beforeState=currentState;
  }

  // Initial click
  if(currentTap !=lastTap){
    if(currentTap==LOW){
      isDeleting=!isDeleting;
      lcd.setCursor(0,0);
      lcd.print("Delete painting?");
      lcd.setCursor(0,1);
      lcd.print("YES");
      lcd.setCursor(3,1);
      lcd.print("          ");
      lcd.setCursor(14,1);
      lcd.print("NO");
      Serial.print(isDeleting);
    }
  lastTap=currentTap;
  if(isDeleting==false){
    joystickSelectYes=false;
    joystickSelectNo=false;
    lcd.clear();
    }
  }

  if(isDeleting==true && Xvalue < 350){
    joystickSelectYes = true;
    joystickSelectNo = false;
  }
  else if(isDeleting==true && Xvalue > 650){
    joystickSelectYes = false;
    joystickSelectNo = true;
  }

  // Selection YES
  if(isDeleting==true && joystickSelectYes==true){
    
    if(printStepNO==0){
      lcd.setCursor(14,1);
      lcd.print("NO");
    }
    
    if(currentMillis-lastMillis >= 500){
      lastMillis=currentMillis;
      if(printStep==0){
        lcd.setCursor(0,1);
        lcd.print("YES");
      }
      else if(printStep==1){
        lcd.setCursor(0,1);
        lcd.print("   ");
      }
      if(hasClicked==true){ // Delete confirm
        lc.clearDisplay(0);
        paintedIndex=0;
        hasClicked=false;
        isDeleting=false;
        lcd.clear();
      }
      printStep++;
      if(printStep > 1){
        printStep=0;
      }
    }
  }
  
  //Selection NO
  if(isDeleting==true && joystickSelectNo==true){
    printStep==0;
    if(printStep==0){
      lcd.setCursor(0,1);
      lcd.print("YES");
    }
    if(currentMillisNo-lastMillisNo >= 500){
      lastMillisNo=currentMillisNo;
      if(printStepNO==0){
        lcd.setCursor(14,1);
        lcd.print("NO");
      }
      else if(printStepNO==1){
        lcd.setCursor(14,1);
        lcd.print("  ");
      }
      if(hasClicked==true){ // Delete decline
        hasClicked=false;
        isDeleting=false;
        lcd.clear();
      }
      printStepNO++;
      if(printStepNO > 1){
        printStepNO=0;
      }
    }
  }
}

  



void buttonLogic(){
  // Check if button has been clicked variables
  static bool hasClicked = false;
  static int lastClick=HIGH;
  int currentClick = digitalRead(Button_pin);
  // Timing variables
  static unsigned long previousTime;
  unsigned long nowTime = millis();
  static unsigned long delayTime = 150;

  // Check if clicked Logic
  if (currentClick != lastClick){
      if (paintedIndex < maxDots){
        if (nowTime-previousTime >=delayTime){

          // Check if spot taken
          bool found = false;
          for(int i=0; i<paintedIndex; i++){
            if(dotRowArray[i]==dotRow && dotColArray[i]==dotCol){
              // Remove dot
              for (int j=i; j<paintedIndex - 1; j++){
                dotRowArray[j] = dotRowArray[j+1];
                dotColArray[j] = dotColArray[j+1];
              }
              paintedIndex--;
              //Turn off LED
              lc.setLed(0,dotRow,dotCol,false);
              found=true;
              break;
            }
          }
        if (found==false){
        dotRowArray[paintedIndex]=dotRow;
        dotColArray[paintedIndex]=dotCol;
        paintedIndex++;
        }
        
        previousTime=nowTime;
} 
}
}
lastClick=currentClick;
}

void LCDinfoScreen(){
  if (isDeleting==false){
    lcd.setCursor(0,0);
    lcd.print("Current Dots:");
    lcd.setCursor(0,1);
    lcd.print(paintedIndex);
  }
}
void matrixDot(){

  int xAxis = analogRead(X_pin); // Current X
  int yAxis = analogRead(Y_pin); // Current Y
  int currentState = digitalRead(SW_pin); //Current click

  // Timing
  static unsigned long lastMoveTime = 0;
  static unsigned long moveDelay = 150;
  unsigned long currentTime = millis();
  // Button click
  static bool hasBeenClicked = false;
  static int lastState = 0;

  // Check if already drawn
  

  // Draw painted dots
  lc.clearDisplay(0);
  for (int i=0;i<paintedIndex;i++){
    lc.setLed(0, dotRowArray[i], dotColArray[i], true);
  }
  // Dot movement speed
  if (currentState != lastState){
    if(currentState==LOW){
      hasBeenClicked=!hasBeenClicked;
      }
    lastState=currentState;
    
  }
  if (hasBeenClicked){
    moveDelay=50;
    }
    else{
      moveDelay=150;
    }
  // Dot Location on screen
  if(currentTime - lastMoveTime >= moveDelay){

    lc.setLed(0,dotRow,dotCol,false);
    if (xAxis <350 && dotRow > 0){ // Move Left
      dotRow--;
      Serial.print("move Left");
      
    }

    if (xAxis > 650 && dotRow < 7){ // Move Right
      dotRow++;
      Serial.print("Move Right");
    }

    if(yAxis < 350 && dotCol < 7){ // Move up
      dotCol++;
      Serial.print("Move Up");
    }
    if(yAxis > 650 && dotCol > 0){ // Move down
      dotCol--;
      Serial.print("Move Down");
    }
    
    lastMoveTime = currentTime;
  }
  lc.setLed(0,dotRow,dotCol,true);
  
    
    // Delete dot on second click
    
    
}

void loop() {
  static unsigned long lastUpdate = 0;
  const unsigned long updateDelay = 1000;
  unsigned long currentUpdate = millis();
  if(currentUpdate - lastUpdate >= updateDelay){
    //Serial.print("Switch:  ");
    //Serial.print(digitalRead(SW_pin));
    //Serial.print("\n");
    //Serial.print("X-axis: ");
    //Serial.print(analogRead(X_pin));
    //Serial.print("\n");
    //Serial.print("Y-axis: ");
    //Serial.println(analogRead(Y_pin));
    //Serial.print("\n\n");
    lastUpdate=currentUpdate;
  }
  if(isDeleting==false){ // Pause func when delete button is pressed
    matrixDot();
    buttonLogic();
    }
  
  deleteButton();
  LCDinfoScreen();
    
  
  
}
