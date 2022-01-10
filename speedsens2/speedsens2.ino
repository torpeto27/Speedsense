/*
  Measure time between (2) IR obstacle sensors and calculate velocity.
  Display velocity on I2C LCD, USB serial, and Bluetooth serial.
  Uses Arduino Teensy++2.0

  Teensy++ 2.0
  Teensy on-board LED on pin 6
  IR Sensors on pins 18 & 19
  Sunfounder I2C LCD1602 Display on pins 0 & 1
  HC-05 Bluetooth on pins 2 (BT tx) & 3 (BT Rx)
  
  HC-05 Bluetooth Module
  VCC to 5V on Teensy
  GND to GND on Teensy
  RXD to pin 3 (txPin)
  TXD to pin 2 (rxPin)

*/

// include the library code
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


const int ledPin = 6; 
const int sen1Pin=18;
const int sen2Pin=19;
const int mmBetween = 100;  // Millimeters between IR sensors
const char title[] = " Speed Detector ";
const char spdHeader[] = "Speed (ft/min): ";
const char tmHeader[] = "Time (ms): ";
const char blankLine[] = "                ";
unsigned long t1=0;
unsigned long t2=0; 
unsigned long maxMicroSec=1000000; // Timeout in us between sensors
unsigned long minMicroSec=15000; // Minimum time in us (reasonable velocity)
float velocity=0;
float dtime=0;

// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  pinMode(sen1Pin,INPUT);
  pinMode(sen2Pin,INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println(title);
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 
  lcd.setCursor(0,0); // set the cursor to column 0, line 0
  lcd.print(spdHeader); // Print a header message to the LCD.
  Serial1.begin(9600);  // setup bt serial on 
  Serial1.println(title);
}

void loop() 
{
  // Wait here until sensor 1 detects object
  while( digitalRead(sen1Pin) );
  // sensor 1 detected object, now watch for detection on sensor 2
  digitalWrite(ledPin, HIGH);   // set the LED on
  t1=micros();
  lcd.setCursor(0,1); // set the cursor to column 0, line 1
  lcd.print(blankLine); // Clear velocity to the LCD.
  while(digitalRead(sen2Pin))
  {
    // Timeout if it is taking too long between sensors
    if ((micros() - t1) > maxMicroSec)
    {
      digitalWrite(ledPin, LOW);    // set the LED off
      return;
    }
  }
  // sensor 2 detected object, calculate the velocity
  t2=micros();
  digitalWrite(ledPin, LOW);    // set the LED off
  dtime=(t2-t1);
  // Ignore if the the time is too low (bad detection) to prevent race condition if 
  // both sensors are covered.
  if ((dtime) < minMicroSec)
    return;
  dtime=dtime/1000/1000; //convert microsecond to second
  velocity=(mmBetween/dtime);  //v=d/t
  velocity=(velocity * 60)/(12*25.4); // convert mm/s to ft/min
  
  //Display the velocity to serial, lcd, and bt
  Serial.print(tmHeader);
  Serial.print(dtime * 1000);
  Serial.print(", ");
  Serial.print(spdHeader);
  Serial.println(velocity);
  lcd.setCursor(0,1); // set the cursor to column 11, line 1
  lcd.print(velocity); // Print velocity to the LCD.
  // LCD will keep displaying the value until there is another detection 
  Serial1.print(tmHeader);
  Serial1.print(dtime * 1000);
  Serial1.print(", ");
  Serial1.print(spdHeader);
  Serial1.println(velocity);
  delay(2000);  // debounce delay
  
}
