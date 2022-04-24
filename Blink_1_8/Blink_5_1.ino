// Illya Kuzmych 2127069 
// Ruvim Piholyuk 2128297 
// CSE 474 Lab 1 22sp
// 5.1

/** Here we make our speaker output a ~250 Hz tone, while LEDs swap every 200 ms
 * The main way this code works is by utilizing the millis() function, 
 * and keeping track of how much time has passed at any given moment
 * By using % 200, we perform an action every 200 ms. 
 * We generated a 250 Hz tone by having the buzzer go to a high 
 * voltage every 4 ms, as 250 Hz is the same as having a period 
 * of 1/250, or 4 out of 1000. 
 */


char buzzer = 2; // variable to hold our pin for later
int state = 0; // initialize a boolean to swap between on and off states of LEDS

void setup() {
  // initialize pin outputs.
 pinMode(buzzer, OUTPUT);
 pinMode(LED_BUILTIN, OUTPUT);
 pinMode(10, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  unsigned int currMillis = millis();
  
  /** Here, the if statement 
   *  checks if 200 ms has passed, 
   *  then changes the state of our LEDs
   */
  if ((currMillis % 200) == 0) { // every 200 ms, changes states
    if (state == 1) {
      state = 0;
    } else {
      state = 1;
    }
  }

  /** If state is 1, then turn on built in LED
   *  and turn off external LED. else, do the opposite
   */
  if (state == 1) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the built-in LED on (HIGH is the voltage level)
    digitalWrite(10, LOW);             // turn the external LED off
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(10, HIGH);
  }

  /** If 4 ms has passed, turn on buzzer, otherwise, 
   *  turn it off. If 10 seconds passes, turn off buzzer
   *  indefinitely
   */
  if (currMillis % 4 == 0 && (currMillis != 10000)) { // turn the speaker off after 10 seconds
    digitalWrite(buzzer, HIGH);
  }
  else {
    digitalWrite(buzzer, LOW);
  }
}
