void setup() {
  // initialize pin outputs.
 pinMode(47, OUTPUT);
 pinMode(48, OUTPUT);
 pinMode(49, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
    digitalWrite(47, HIGH);
    digitalWrite(49, LOW);
    delay(333);
    digitalWrite(47, LOW);
    digitalWrite(48, HIGH);
    delay(333);
    digitalWrite(48, LOW);
    digitalWrite(49, HIGH);
    delay(333);
}
