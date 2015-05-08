#include <avr/interrupt.h>
#include <avr/sleep.h>

/* Sleep Demo
 * ------------
 * Example code to demonstrate the sleep functions in a Arduino.
 *
 * use a pull up resistor on pin 2 and 12 to 5V.
 * attach a led with resistor to gnd on pin 10.
 * ground pin 12 momentary to put the Arduino to sleep
 * and ground pin 2 momentary to wake it up again. 
 *
 * When awake, the arduino will run the led_blink code 
 * from the example sketchbook, Created 1 June 2005
 * by DojoDave <http://www.0j0.org>
 * http://arduino.berlios.de
 * who based it on an orginal by H. Barragan for the Wiring i/o board
 *
 * Hacked together by MacSimski 30-12-2006.
 */

int ledPin = 13;            // LED connected to digital pin 13
int sleepPin = 12;          // active LOW, ground this pin momentary to sleep
int interruptPin = 8;      // LED to show the action of a interrupt
int wakePin = 2;            // active LOW, ground this pin momentary to wake up
int sleepStatus = 0;        // variable to store a request for sleep

void setup()
{
  pinMode(ledPin, OUTPUT);         // sets the digital pin as output
  pinMode(interruptPin, OUTPUT);   //
  pinMode(sleepPin, INPUT);        // sets the digital pin as input
  pinMode(wakePin, INPUT);

  /* Now is time to enable a interrupt. In the function call 
   * attachInterrupt(A, B, C)
   * A   can be either 0 or 1 for interrupts on pin 2 or 3.   
   * 
   * B   Name of a function you want to execute while in interrupt A.
   *
   * C   Trigger mode of the interrupt pin. can be:
   *             LOW        a low level trigger
   *             CHANGE     a change in level trigger
   *             RISING     a rising edge of a level trigger
   *             FALLING    a falling edge of a level trigger
   *
   * In all but the IDLE sleep modes only LOW can be used.
   */

  attachInterrupt(0, wakeUpNow, LOW); // use interrupt 0 (pin 2) and run function
                                      // wakeUpNow when pin 2 gets LOW 
}



void sleepNow()         // here we put the arduino to sleep
{
    /* Now is the time to set the sleep mode. In the Atmega8 datasheet
     * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
     * there is a list of sleep modes which explains which clocks and 
     * wake up sources are available in which sleep modus.
     *
     * In the avr/sleep.h file, the call names of these sleep modus are to be found:
     *
     * The 5 different modes are:
     *     SLEEP_MODE_IDLE         -the least power savings 
     *     SLEEP_MODE_ADC
     *     SLEEP_MODE_PWR_SAVE
     *     SLEEP_MODE_STANDBY
     *     SLEEP_MODE_PWR_DOWN     -the most power savings
     *
     * For now, we want as much power savings as possible, 
     * so we choose the according sleep modus: SLEEP_MODE_PWR_DOWN
     * 
     */  
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here

    sleep_enable();              // enables the sleep bit in the mcucr register
                                 // so sleep is possible. just a safety pin 

    /* Now is time to enable a interrupt. we do it here so an 
     * accidentally pushed interrupt button doesn't interrupt 
     * our running program. if you want to be able to run 
     * interrupt code besides the sleep function, place it in 
     * setup() for example.
     * 
     * In the function call attachInterrupt(A, B, C)
     * A   can be either 0 or 1 for interrupts on pin 2 or 3.   
     * 
     * B   Name of a function you want to execute at interrupt for A.
     *
     * C   Trigger mode of the interrupt pin. can be:
     *             LOW        a low level triggers
     *             CHANGE     a change in level triggers
     *             RISING     a rising edge of a level triggers
     *             FALLING    a falling edge of a level triggers
     *
     * In all but the IDLE sleep modes only LOW can be used.
     */

    attachInterrupt(0,wakeUpNow, LOW); // use interrupt 0 (pin 2) and run function
                                       // wakeUpNow when pin 2 gets LOW 

    sleep_mode();                // here the device is actually put to sleep!!
                                 // 

    sleep_disable();             // first thing after waking from sleep:
                                 // disable sleep...
    detachInterrupt(0);          // disables interrupt 0 on pin 2 so the 
                                 // wakeUpNow code will not be executed 
                                 // during normal running time.
    delay(1000);                 // wat 2 sec. so humans can notice the
                                 // interrupt. 
                                 // LED to show the interrupt is handled
    digitalWrite (interruptPin, LOW);      // turn off the interrupt LED

}



void wakeUpNow()        // here the interrupt is handled after wakeup
{
  //execute code here after wake-up before returning to the loop() function
  // timers and code using timers (serial.print and more...) will not work here.
  digitalWrite(interruptPin, HIGH);
}




void loop()
{
  digitalWrite(ledPin, HIGH);            // sets the LED on
  delay(1000);                           // waits for a second
  digitalWrite(ledPin, LOW);             // sets the LED off
  delay(1000);                           // waits for a second
  sleepStatus = digitalRead(sleepPin);   // read sleep pin here. only active 
                                         //when blink led is off.
  if (sleepStatus == LOW) {            // start to put the device in sleep
      sleepNow();                      // sleep function called here
      }
}
