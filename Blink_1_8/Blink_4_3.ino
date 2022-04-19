
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(10, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(2, HIGH);
  digitalWrite(10, HIGH);   // turn the external LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(2, LOW);
  delay(200);    // wait for a second
  digitalWrite(2, HIGH);
  digitalWrite(10, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(2, LOW);
  delay(200);                       // wait for a second
}
