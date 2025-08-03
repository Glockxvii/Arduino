//www.elegoo.com
//2016.12.08

int redLedPin = 12;
int greenLedPin = 11;
int blueLedPin = 10;
int redPin = 9;
int greenPin = 8;
int bluePin =7;
int buttonRed = 4;
int buttonGreen= 3;
int buttonBlue = 2;

bool isRedOn = false;
int prevRedState = HIGH;

bool isGreenOn = false;
int prevGreenState = HIGH;

bool isBlueOn = false;
int prevBlueState = HIGH;

byte leds = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buttonRed, INPUT_PULLUP);  
  pinMode(buttonGreen, INPUT_PULLUP);
  pinMode(buttonBlue, INPUT_PULLUP);  
}

void loop() 
{
  int currentRedState = digitalRead(buttonRed);
  
  // Red LED
  if (prevRedState == HIGH && currentRedState == LOW) {
    isRedOn = !isRedOn;
    delay(200);
  }
  
  if (isRedOn == true) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(redPin, HIGH);
  }
  if (isRedOn == false) {
    digitalWrite(redLedPin, LOW);
    digitalWrite(redPin, LOW);
  }
  prevRedState = currentRedState;
  
  //Green LED
  int currentGreenState = digitalRead(buttonGreen);
  

  if (prevGreenState == HIGH && currentGreenState == LOW) {
    isGreenOn = !isGreenOn;
    delay(200);
  }
  
  if (isGreenOn == true) {
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(greenPin, HIGH);
  }
  if (isGreenOn == false) {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(greenPin, LOW);
  }
  prevGreenState = currentGreenState;
     

  //Blue LED bool isBlueOn = false;
            //int prevBlueState = HIGH;

  int currentBlueState = digitalRead(buttonBlue);

  if (currentBlueState == LOW && prevBlueState == HIGH) {
    isBlueOn = !isBlueOn;
    delay(200);
  }

  if (isBlueOn == true) {
    digitalWrite(blueLedPin, HIGH);
    digitalWrite(bluePin, HIGH);
  }
  if (isBlueOn == false) {
    digitalWrite(blueLedPin, LOW);
    digitalWrite(bluePin, LOW);
  }

  prevBlueState = currentBlueState;
}
