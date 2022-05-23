#define NOTE_1 16000000 / (16 * 293) - 1
#define NOTE_2 16000000 / (16 * 329) - 1
#define NOTE_3 16000000 / (16 * 261) - 1
#define NOTE_4 16000000 / (16 * 130) - 1
#define NOTE_5 16000000 / (16 * 196) - 1
#define NOTE_rest 0

/**
 * Pin 2: A
 * Pin 3: B
 * Pin 4: C
 * Pin 5: D
 * Pin 6: E
 * Pin 7: F
 * Pin 8: G
 * Pin 9: DS4
 * Pin 10: DS3
 * Pin 11: DS2
 * Pin 12: DS1
 */

/* array defining all the frequencies of the melody  */
int melody[] = { NOTE_1, NOTE_rest, NOTE_2, NOTE_rest, NOTE_3, NOTE_rest, NOTE_4, NOTE_rest, NOTE_5, NOTE_rest };
int curr = 0;
int sleep;
unsigned long timer;


void setup() {
  DDRL |= 1 << DDL2;
  
  /* setup for the 4-digit 7-segment display by setting appropriate pins to outputs */
  DDRE |= 1 << DDE4; // pin 2
  DDRE |= 1 << DDE5; // pin 3
  DDRG |= 1 << DDG5; // pin 4
  DDRE |= 1 << DDE3; // pin 5
  DDRH |= 1 << DDH3; // pin 6
  DDRH |= 1 << DDH4; // pin 7
  DDRH |= 1 << DDH5; // pin 8
  DDRH |= 1 << DDH6; // pin 9
  DDRB |= 1 << DDB4; // pin 10
  DDRB |= 1 << DDB5; // pin 11
  DDRB |= 1 << DDB6; // pin 12
  
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
  PORTH |= 1 << PORTH6; // turn on DS4
  PORTB |= 1 << PORTB4; // turn on DS3
  PORTB |= 1 << PORTB5; // turn on DS2
  PORTB |= 1 << PORTB6; // turn on DS1

  // 0
  PORTE &= ~(1 << PORTE4); // turn on A
  PORTE &= ~(1 << PORTE5); // turn on B
  PORTG &= ~(1 << PORTG5); // turn on C
  PORTE &= ~(1 << PORTE3); // turn on D
  PORTH &= ~(1 << PORTH3); // turn on E
  PORTH &= ~(1 << PORTH4); // turn on F
  PORTH |= 1 << PORTH5;    // turn off G
  delay(1000);

  // 9
  PORTE &= ~(1 << PORTE4); // turn on A
  PORTE &= ~(1 << PORTE5); // turn on B
  PORTG &= ~(1 << PORTG5); // turn on C
  PORTE &= ~(1 << PORTE3); // turn on D
  PORTH |= 1 << PORTH3; // turn off E
  PORTH &= ~(1 << PORTH4); // turn on F
  PORTH &= ~(1 << PORTH5);    // turn on G
  delay(1000);
  
  /*
  task2(1);
  timer++;
  delay(1);
  */
}
