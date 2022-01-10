//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <EnableInterrupt.h>
#include <Bounce.h>

volatile unsigned long mycount=0;
unsigned long t1=0;
unsigned long t2=0; 

// Instantiate a Bounce object with a 4 millisecond debounce time
Bounce bouncer = Bounce( 18,4 );

void setup() {
  // put your setup code here, to run once:
  attachInterrupt(digitalPinToInterrupt(18), myint1, RISING);
  attachInterrupt(19, myint2, CHANGE);
  pinMode (18, INPUT_PULLUP);
  pinMode (19, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
//  Serial.println(mycount)
 // Update the debouncer
  bouncer.update ( );
}

void myint1()
{
  if (bouncer.read())
  {
    mycount++;
  }
  t1=micros();
  Serial.println(mycount);
  delay(1000);
  return;
}

void myint2()
{
  t2=micros();
  if ((t2-t1) < 1000000)
  {
    Serial.println("2nd exp");
  }
  delay(1000);
  return;
}
