#define NOTE_1 16000000 / (16 * 293) - 1
#define NOTE_2 16000000 / (16 * 329) - 1
#define NOTE_3 16000000 / (16 * 261) - 1
#define NOTE_4 16000000 / (16 * 130) - 1
#define NOTE_5 16000000 / (16 * 196) - 1
#define NOTE_rest 0

/* array defining all the frequencies of the melody  */
int melody[] = { NOTE_1, NOTE_rest, NOTE_2, NOTE_rest, NOTE_3, NOTE_rest, NOTE_4, NOTE_rest, NOTE_5, NOTE_rest };
int curr = 0;
int sleep;
unsigned long timer;


void setup() {
  DDRL |= 1 << DDL2;

  noInterrupts();
  TCCR4A = B01010100; // bottom two bits 0 (WGMn1 & WGMn0)
  TCCR4B = B00001001; // 4th bit set to 1 (WGMn4 & WGMn3) and set bottom bit for clock select
  DDRH |= 1 << DDH3; // pin 6
  interrupts();
  
}

/** 
 *  Simple pattern of turning on LED for 
 *  250 ms, off for 750 ms, then repeating
 */
void task1(int on) {
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

/**
 * Playing a song for Task 2...
 */
void task2(int on) {
 if (on && !sleep) {
    OCR4A = melody[curr % 10];
    if (timer % 1000 == 0 && !(melody[curr % 10] == NOTE_rest)) {
      curr++;
    }
    else if ((melody[curr % 10] == NOTE_rest) && (timer % 2000 == 0)) {
      curr++;
    }
    if ( curr == 10 ) {
      sleep = 1;
      curr = 0;
      OCR4A = 0;
    }
  }
  else {
    OCR4A = 0;
    curr = 0;
  }
  if (sleep == 1 && timer % 6000 == 0) {
    sleep = 0;
  }
}


void loop() {
  task2(1);
  timer++;
  delay(1);
}
