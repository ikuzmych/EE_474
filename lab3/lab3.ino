unsigned long timer = 0;


void setup() {
  DDRL |= 1 << DDL2;
  
}

void task1() {
  if (timer < 250) {
    PORTL |= 1 << PORTL2;
  } else {
    PORTL &= ~(1 << PORTL2);
  }
  if (timer == 1000) {
    timer = 0;
  }
  timer++;
}


void loop() {
  task1();
}
