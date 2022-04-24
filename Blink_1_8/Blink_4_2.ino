// Illya Kuzmych 2127069 
// Ruvim Piholyuk 2128297 
// CSE 474 Lab 1 22sp
// 4.2

/** This code adds a speaker that buzzes alongside the 
 *  LED blinking
 *  Pin 10: external LED
 *  Pin 2: 8 Ohm speaker
 */ 


void setup() {
  // initialize pin outputs.
  pinMode(10, OUTPUT);
  pinMode(2, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(10, HIGH);   // turn the LED on
  digitalWrite(2, HIGH);    // click the speaker
  delay(200);                       // wait for 1/5 second
  digitalWrite(10, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(2, LOW);
  delay(200);                       // wait for 1/5 second
}
