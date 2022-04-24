// Illya Kuzmych 2127069 
// Ruvim Piholyuk 2128297 
// CSE 474 Lab 1 22sp
// 4.3

/** Here is our code for section 4.3 of the lab
 * Our main change was to have the speaker go to high and low before any delay
 * then turn back to high once the delay ends. This way, the speaker clicks
 * when the lights swap.
 * Pin 2: output to speaker
 * Pin 10: output to external LED
 * LED_BUILTINL: goes to built-in LED labeled as "L" on the Mega board
 */


void setup() {
  // initialize the three pins we want as outputs.
  pinMode(10, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(2, HIGH); // click the speaker 
  digitalWrite(10, HIGH);   // turn the external LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, LOW); // turn the built-in LED on (HIGH is the voltage level)
  digitalWrite(2, LOW);
  delay(200);    // wait for a 1/5 second
  
  digitalWrite(2, HIGH); // click the speaker back on once the LEDS change
  digitalWrite(10, LOW);    // turn the external LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, HIGH); // turn the build-in LED off by making the voltage LOW
  digitalWrite(2, LOW);
  delay(200);                       // wait for a 1/5 second
}
