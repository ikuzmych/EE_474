unsigned long timer;


void setup() {
  DDRL |= 1 << DDL2;
}

void task1() {
  if (timer == 1) {
    PORTL |= 1 << PORTL2;
  } 
  if (timer == 250) {
    PORTL &= ~(1 << PORTL2);
  }
  if (timer > 1000) {
    timer = 0;
  }
  timer++;
  delay(1);
}


void loop() {
  task1();
}
