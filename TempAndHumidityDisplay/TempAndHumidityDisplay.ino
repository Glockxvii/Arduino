#include <LiquidCrystal.h>

#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

// LCD pins (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);


float temperature;
float humidity;
float lastTemp;
float lastHumidity;
bool screenCleared;

void setup( )
{
  lcd.begin(16, 2); // initialize the lcd (16 rows 2 colums)
  Serial.begin(9600);
}

void tempReading(float &temperature, float &humidity) {
  
  static unsigned long lastTime;
  unsigned long currentTime = millis();
  static unsigned long delayTime = 1000ul;
  if(currentTime - lastTime >= delayTime) {
    lastTime = currentTime;
    dht_sensor.measure(&temperature, &humidity);
    Serial.print("Temp: ");
    Serial.print(temperature, 1);
    Serial.println(" C");
    Serial.print("Humidity: ");
    Serial.print(humidity, 1);
    Serial.println("%");
  }
}

void lcdDisplay() {
  //After how much time set isUpdating back to false
  unsigned long flashTime = millis();
  static unsigned long lastFlash;
  static int flashDuration = 2000;
  static int dotCount = 0;
  static bool isUpdating = false;
      
  if ((abs(temperature - lastTemp) > 0.1 || abs(humidity - lastHumidity)  > 0.1) && !isUpdating){
    lastTemp = temperature;
    lastHumidity = humidity;
    isUpdating = true;
    lastFlash=flashTime;
  }

  if(isUpdating==true){
      if(screenCleared==false){
        lcd.clear();
        screenCleared=true;
      }
      lcd.setCursor(0,0);
      lcd.print("update");

    if(flashTime-lastFlash >= flashDuration){
        isUpdating=false;
      }
    }  
      
    
  else {
    if(screenCleared==true){
      lcd.clear();
      screenCleared=false;
    }
    lcd.setCursor(0,0);
    lcd.print("Temp: ");
    lcd.print(temperature, 1);
    lcd.print(" C");
    lcd.setCursor(0,1);
    lcd.print("Humidity: ");
    lcd.print(humidity, 1);
    lcd.print("%");
    }
    
  }
  


void loop( )
{
  
  tempReading(temperature, humidity);
  lcdDisplay();
}
