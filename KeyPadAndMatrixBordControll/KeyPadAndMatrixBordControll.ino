#include <LedControl.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

// LCD pins (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Keypad Setup
const byte ROWS = 4;
const byte COLS = 4;

// Connect rows to arduino pins
byte rowPins[ROWS] = {2, 3, 4, 5};

// Connect colums to arduino pins
byte colPins[COLS] = {6, A0, A1, A2};


// Connect LCD Matrix Display
int DIN_PIN = A3; // Data in
int CS_PIN = A4; // Chip Select (Load)
int CLK_PIN = A5; // Clock

// Initialize LedControl with the pins
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1); // 1 device

// Define keys on the keypad
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Create the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Variables
int cursorPos = 0;  // Position of user input
char buffer[16]; // Array of input characters + 1 for null termiantor '\0'
int bufferLength = 0; // Track how many input keys are stored
bool screenFullMessage = true;
//bool overwritingMessage = true;
bool ledState = true;

//Matrix Bord variables
int lcdColPos = 0; //LCD Matrix Board Column Position
int moveLeft = 0;
int moveRight = 0;
int moveUp = 0;
int moveDown = 0;


// Timing integers
unsigned long nowTime; // non negative numbers going to almost 5bil. used with millis()
unsigned long lastTime = 0;
unsigned long waitTime = 500;

void appendKey(char key) {
  if (bufferLength < 16) {
    buffer[bufferLength] = key; // append key input
    bufferLength++; // increase length
    buffer[bufferLength] = '\0'; // Adds a STOP sign after each input, so the computer knows to stop reading after the last input
  }
  else if (bufferLength == 16) {
    for (int i = 15; i > 0; i--) {
      buffer[i] = buffer[i - 1];
    }
  buffer[0] = key;
  
  }
  
}



void screenFullWarning() {

  for (int i=0; i < 5; i++) {
  lcd.setCursor(0, 0);
  lcd.print("Screen Full.");
  delay(200);

  lcd.setCursor(0, 0);
  lcd.print("                ");
  delay(50);

  lcd.setCursor(0, 0);
  lcd.print("Screen Full..");
  delay(200);

  lcd.setCursor(0, 0);
  lcd.print("                ");
  delay(50);

  lcd.setCursor(0, 0);
  lcd.print("Screen Full...");
  delay(200);

  lcd.setCursor(0, 0);
  lcd.print("                ");
  delay(100);
  }
  
  lcd.setCursor(0, 0);
  lcd.print("Press any Key");
  delay(1000);

  lcd.setCursor(0, 0);
  lcd.print("                ");
  delay(100);

  lcd.setCursor(0, 0);
  lcd.print("To Overwrite");
  delay(1000);

}

void lcdDisplayLogic(char key) {
  if (key) {
    if (moveUp < 8 && moveLeft < 8) {
      lcd.setCursor(0,0);
      lcd.print("Current Position"); // Displays this text
      lcd.setCursor(0, 1); // Set where next action will take place, second row first column
      lcd.print("Y-: ");
      lcd.print(moveUp);
      lcd.print("    X-: ");
      lcd.print(moveLeft);
    }
    else {
      lcd.setCursor(0,0);
      lcd.print("                ");
      lcd.setCursor(0,0);
      lcd.print("Out Of Bounds!");
      lcd.setCursor(0, 1);
      lcd.print("Y-: ");
      lcd.print(moveUp);
      lcd.print("    X-: ");
      lcd.print(moveLeft);
    }
     
  }

}


void lcdMatrixLogic(char key) {

  if(key) {
    if(key=='4') { //col
      if (moveLeft > 0) {
        moveLeft--;
        moveRight=moveLeft;
        
        Serial.println(moveLeft);
      }
    
    }
    if(key=='6') { //col
      if (moveRight < 8){
        moveRight++;
        moveLeft=moveRight;
        
        Serial.println(moveRight);
      }
    }
    if(key=='2') { //row
      if (moveUp < 8){
        moveUp++;
        moveDown=moveUp;
        
        Serial.println(moveUp);
      }
    }
    if(key=='8'){ //row
      if (moveDown > 0){
        moveDown--;
        moveUp=moveDown;
        Serial.println(moveDown);
      }
    }
  }

  
  if (nowTime - lastTime >= waitTime) {
    lastTime=nowTime;
    lc.clearDisplay(0);
    lc.setLed(0,moveLeft,moveUp,true);
    
  }

}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); // initialize the lcd (16 rows 2 colums)
  lc.shutdown(0, false); // Wake up the LCD Matrix display, 0 = first device, false = turn off shutdown mode
  lc.setIntensity(0, 0); // Brightness (0-15) 0=first device
  lc.clearDisplay(0); // Clear Display, 0=first device

}

void loop() {
  char key = keypad.getKey(); // This runs in loop, and is passed to both functions here. Otherwise if called from both functions seperately, it will lag, couse only one can get an answer at once.
  nowTime = millis();
  lcdMatrixLogic(key);
  lcdDisplayLogic(key);
  
  
  
  
      
}

  


