// Define pins
  #define BLUE 3
  #define GREEN 5
  #define RED 6
  int red = 0;
  int green = 0;
  int blue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode (RED, OUTPUT);
  pinMode (GREEN, OUTPUT);
  pinMode (BLUE, OUTPUT);
  red = 0;
  green = 0;
  blue = 0;
  
}

void loop() {
  // put your main code here, to run repeatedly:
  red++;
  if (red >= 255) {red = 0; blue++;}
  if (blue >= 255) {blue = 0; green++;}
  if (green >= 255) {green = 0;}
  analogWrite(RED, red);
  analogWrite(BLUE, blue);
  analogWrite(GREEN, green);
  Serial.print("Red: ");
  Serial.println(red);
  Serial.print("Blue: ");
  Serial.println(blue);
  Serial.print("Green: ");
  Serial.println(green);
  delay(20);

}
