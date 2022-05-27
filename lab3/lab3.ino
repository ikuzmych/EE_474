#define NOTE_1 16000000 / (2 * 293) - 1
#define NOTE_2 16000000 / (2 * 329) - 1
#define NOTE_3 16000000 / (2 * 261) - 1
#define NOTE_4 16000000 / (2 * 130) - 1
#define NOTE_5 16000000 / (2 * 196) - 1
#define NOTE_rest 0
#define DELAY_TIMEOUT_VALUE 16000000 / (2 * 500) - 1
#define DONE 1
#define PENDING 0


byte seven_seg_digits[10] = { 0b11111100, // 0
                              0b01100000, // 1
                              0b11011010, // 2
                              0b11110010, // 3
                              0b01100110, // 4
                              0b10110110, // 5 
                              0b10111110, // 6
                              0b11100000, // 7
                              0b11111110, // 8
                              0b11100110 }; // 9
/**
 * Pin 30: A
 * Pin 31: B
 * Pin 32: C
 * Pin 33: D
 * Pin 34: E
 * Pin 35: F
 * Pin 36: G
 * Pin 37: DP
 * Pin 23: DS4
 * Pin 24: DS3
 * Pin 25: DS2
 * Pin 26: DS1
 */

/* array defining all the frequencies of the melody  */
long melody[] = { NOTE_1, NOTE_rest, NOTE_2, NOTE_rest, NOTE_3, NOTE_rest, NOTE_4, NOTE_rest, NOTE_5, NOTE_rest };
int curr = 0;
int sleep;
unsigned long timer;
unsigned long counter = 0;
volatile int sFlag = PENDING;
void setup() {

  DDRA = 0b00011110;
  DDRC = 0xFF;
  
  noInterrupts();
  TCCR4A = B01010100; // bottom two bits 0 (WGMn1 & WGMn0)
  TCCR4B = B00001001; // 4th bit set to 1 (WGMn4 & WGMn3) and set bottom bit for clock select
  DDRH |= 1 << DDH3; // pin 6

  /* for the delay timer */
  TCCR3A = B01010100; // bottom two bits 0 (WGMn1 & WGMn0)
  TCCR3B = B00001001; // 4th bit set to 1 (WGMn4 & WGMn3) and set bottom bit for clock select (prescaler of 1), clear timer on compare
  TIMSK3 = B00000010; // enable compare match interrupt for TIMER3
  OCR3A = DELAY_TIMEOUT_VALUE;
  interrupts();
  
}

ISR(TIMER3_COMPA_vect) {
  sFlag = DONE;
}

/** 
 *  Simple pattern of turning on LED for 
 *  250 ms, off for 750 ms, then repeating
 */
int x = 0;
void task1(int on) {
  if (x < 250) {
    PORTL |= 1 << PORTL2;
  } else {
    PORTL &= ~(1 << PORTL2);
  }
  if (x == 1000) {
    x = 0;
  }
  x++;
  return;
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
  return;
}

void task3 (int on) {
  if (on) {
    if (timer % 100 == 0) counter++;
    
    if (timer % 4 == 0) {
      PORTC = 0b00000000;
      PORTA = 0b11111101;
      PORTC = seven_seg_digits[counter % 10];
    }
    else if (timer % 4 == 1) {
      PORTC = 0b00000000;
      PORTA = 0b11111011;
      PORTC = seven_seg_digits[(counter / 10) % 10];
      PORTC |= 0b00000001;
    }
    else if (timer % 4 == 2) {     
      PORTC = 0b00000000;
      PORTA = 0b11110111;
      PORTC = seven_seg_digits[(counter / 100) % 10];
    }
    else if (timer % 4 == 3) {
      PORTC = 0b00000000;
      PORTA = 0b11101111;
      PORTC = seven_seg_digits[(counter / 1000) % 10];
    }
  } else {
    PORTA = 0b11111111;
  }
  return;
}


void loop() {
  if (sFlag == DONE) {
    timer++; 
    task1(1);
    task2(1);
    task3(1);
    sFlag = PENDING;   
  }
}
