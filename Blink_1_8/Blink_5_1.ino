/*
char buzzer = 2;
int state = 0;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
 pinMode(buzzer, OUTPUT);
 pinMode(LED_BUILTIN, OUTPUT);
 pinMode(10, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  unsigned int currMillis = millis();
  if (currMillis % 1000 == 0) {
    if (state == 1) {
      state = 0;
    } else {
      state = 1;
    }
  } 
  if (state == 1) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(10, LOW);
  }                   // wait for a second
  else {
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(10, HIGH);
  }                      // wait for a second
  
  if (currMillis % 4 == 0) {
    digitalWrite(buzzer, HIGH);
  }
  else {
    digitalWrite(buzzer, LOW);
  }
  
}*/
