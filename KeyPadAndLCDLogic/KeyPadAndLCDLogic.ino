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

void lcdDisplayLogic() {
  char key = keypad.getKey();
  if (key) {
    lcd.clear(); // Clears current screen
    lcd.print("Key(s) pressed:"); // Displays this text
    appendKey(key); // Append the input to Buffer Array
    lcd.setCursor(cursorPos, 1); // Set where next action will take place, second row first column
    lcd.print(buffer); // Print What has been appended

    if (bufferLength >= 16 && screenFullMessage == true) {
      screenFullWarning();
      screenFullMessage = false;
    }

    else if (bufferLength >= 16) {
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      delay(10);
      lcd.print("Overwriting...");
    }

  }

}

void lcdMatrixLogic() {

  lc.setLed(0, 0, 0, true); // (first device, row 0, column 0, on or off)
  delay(2000);
  lc.setLed(0, 0, 0, false);
  delay(1000);
}

void setup() {
  lcd.begin(16, 2); // initialize the lcd (16 rows 2 colums)
  lcd.print("Press keys:"); // What appears on lcd

  lc.shutdown(0, false); // Wake up the LCD Matrix display, 0 = first device, false = turn off shutdown mode
  lc.setIntensity(0, 8); // Brightness (0-15) 0=first device
  lc.clearDisplay(0); // Clear Display, 0=first device

}

void loop() {
  lcdDisplayLogic();
  //lcdMatrixLogic();
      
}

  


