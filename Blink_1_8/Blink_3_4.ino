// Illya Kuzmych 2127069 
// Ruvim Piholyuk 2128297 
// CSE 474 Lab 1 22sp
// 3.4

/** This code has identical behavior to that of 2.2
 * except now we write out to pin 10 which is 
 * hooked up to an external LED on a breadboard
 */
 
//3.4
// change the output from LED_BUILTIN to pin 10 
void setup() {
  // initialize digital pin 10 as an output.
  pinMode(10, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(10, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);                       // wait for 1/5 second
  digitalWrite(10, LOW);    // turn the LED off by making the voltage LOW
  delay(200);                       // wait for 1/5 second
}
