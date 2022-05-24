#define NOTE_1 16000000 / (2 * 293) - 1
#define NOTE_2 16000000 / (2 * 329) - 1
#define NOTE_3 16000000 / (2 * 261) - 1
#define NOTE_4 16000000 / (2 * 130) - 1
#define NOTE_5 16000000 / (2 * 196) - 1
#define NOTE_rest 0
#define N_TASKS 3
#define READY 0
#define RUNNING 1
#define SLEEPING 2

void (*taskPointers[N_TASKS]) (void *p);
int state[N_TASKS];
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
int melody[] = { NOTE_1, NOTE_rest, NOTE_2, NOTE_rest, NOTE_3, NOTE_rest, NOTE_4, NOTE_rest, NOTE_5, NOTE_rest };
int curr = 0;
int sleep;
unsigned long timer;
unsigned long counter = 0;

void setup() {
  taskPointers[0] = &task1;
  
  DDRA = 0b00011110;
  DDRC = 0xFF;
  
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
void task1(void *p) {
  if (timer % 1000 == 0) {
    PORTL |= 1 << PORTL2;
  } 
  if (timer % 1250 == 0) {
    PORTL &= ~(1 << PORTL2);
  }
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
}


void loop() {
  timer++;
  task1(p);
  task2(1);
  task3(1);
  delay(1);
}
