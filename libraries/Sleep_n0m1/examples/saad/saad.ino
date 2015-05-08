#include <Sleep_n0m1.h>

Sleep sleep;
unsigned long sleepTime; //how long you want the arduino to sleep

const int ledPin =  13;      // the number of the LED pin
int ledState = HIGH;             // ledState used to set the LED

void setup()
{
   
   Serial.begin(9600);
   sleepTime = 60000; //set sleep time in ms, max sleep time is 49.7 days


   pinMode(ledPin, OUTPUT);      
   
}

void loop()
{
  
  delay(100); ////delays are just for serial print, without serial they can be removed
  Serial.println("execute your code here");
  digitalWrite(ledPin, ledState);

  Serial.print("sleeping for ");
  Serial.println(sleepTime); 
  delay(100); //delay to allow serial to fully print before sleep
    
  sleep.pwrDownMode(); //set sleep mode
  sleep.sleepDelay(sleepTime); //sleep for: sleepTime
  digitalWrite(ledPin, !ledState);
  sleep.pwrDownMode(); //set sleep mode
  sleep.sleepDelay(sleepTime); //sleep for: sleepTime
  digitalWrite(ledPin, ledState);
 
}
