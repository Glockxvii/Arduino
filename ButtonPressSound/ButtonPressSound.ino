#include <pitches.h>

//www.elegoo.com
//2016.12.08

int buzzer = 12;//the pin of the active buzzer
void setup()
{
 pinMode(buzzer,OUTPUT);//initialize the buzzer pin as an output
}
void loop()
{
 unsigned char i;
 while(1)
 {
   tone(12, 1000, 50); //pin, frequency, miliseconds
   delay(3000);
   
 }
  
} 
