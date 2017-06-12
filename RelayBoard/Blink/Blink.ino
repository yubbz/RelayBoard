/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  pinMode(0x04, OUTPUT);
  pinMode(0x0C, OUTPUT);
  pinMode(0x0B, OUTPUT);
  pinMode(0x07, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(20, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(13,  HIGH);   // turn the LED on (HIGH is the voltage level)


digitalWrite(8, HIGH);   // turn the LED on (HIGH is the voltage level)
delay(1000); 
digitalWrite(7, HIGH);   // turn the LED on (HIGH is the voltage level)
delay(1000); 
digitalWrite(6, HIGH);   // turn the LED on (HIGH is the voltage level)
delay(1000); 
digitalWrite(5, HIGH);   // turn the LED on (HIGH is the voltage level)
delay(1000); 

  
  delay(1000);             // wait for a second
  digitalWrite(13,  LOW);   // turn the LED on (LOW is the voltage level)
  digitalWrite(8,  LOW);
  digitalWrite(7,  LOW);
  digitalWrite(6,  LOW);
  digitalWrite(5,  LOW);
  delay(1000);              // wait for a second
}
