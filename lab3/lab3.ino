/* Lab_3.ino
 * @file   Lab_3_sync.ino
 *   @author    Ruvim Piholyuk, Illya Kuzmych
 *   @date      26-May-2022
 *   @brief   Demo 1 according to spec
 *   
 *  Lab_3.ino demonstrates both task 1, 2, and 3
 *  with the use of a RR scheduler.
 *  
 *  Pin 47 is used to toggle an LED on for 250 ms 
 *  and off for 750ms repeating every second.
 *  
 *  Pin 6 is used to play the theme of "Close Encounters
 *  of the Third Kind" through a speaker. Every note is played
 *  for one second and there is a 2 second pause between notes.
 *  Once all notes are played the task sleeps for 4 seconds 
 *  until it repeats.
 *  
 */
// Defines for OCRA4 to play notes from "Close Encounters"
#define NOTE_1 16000000 / (2 * 293) - 1    ///< to output 293 Hz to Pin 6
#define NOTE_2 16000000 / (2 * 329) - 1    ///< to output 329 Hz to Pin 6
#define NOTE_3 16000000 / (2 * 261) - 1    ///< to output 261 Hz to Pin 6
#define NOTE_4 16000000 / (2 * 130) - 1    ///< to output 130 Hz to Pin 6
#define NOTE_5 16000000 / (2 * 196) - 1    ///< to output 196 Hz to Pin 6
#define NOTE_rest 0                        ///< to output 0 Hz to Pin 6



/// byte Array to indicate what number to display on the 7 seg displays corresponding to the index of the Array
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

/// array defining all the frequencies of the melody
long melody[] = { NOTE_1, NOTE_rest, NOTE_2, NOTE_rest, NOTE_3, NOTE_rest, NOTE_4, NOTE_rest, NOTE_5, NOTE_rest};
int curr = 0;                  ///< int to hold the index of the array accessing an index in the melody array
unsigned long timer = 0;       ///< int to hold the current time since the program has started
unsigned long counter = 0;     ///< int to hold the amount of times 100ms have passed
int sleep = 0;
/**
 * setup to intialize ports to the desired specifications
 *
 * Initializing taskPointers to hold task1, task2, schedule_sync, and null in that order.
 * 
 * Set port A 1-5 to putput
 * Set port C to output
 * Set Pin 47 to output
 * Set Pin 6 to output
 * 
 * Set Counter 4 to no prescaler and to reset when overflowed
 * Set Counter 3 to count for 2 ms
 *
 */
void setup() {

  DDRA = 0b00011110;
  DDRC = 0xFF;
  
  noInterrupts();
  TCCR4A = B01010100; ///< bottom two bits 0 (WGMn1 & WGMn0)
  TCCR4B = B00001001; ///< 4th bit set to 1 (WGMn4 & WGMn3) and set bottom bit for clock select
  DDRH |= 1 << DDH3; ///< pin 6
  interrupts();
  
}

int x = 0;
/**
 * task1 toggles an LED at pin 6 on for 250 ms and 750 ms with a 1 second period
 *
 * @param int on is to turn on task1
 */
void task1(int on) {
  if (x < 250) {
    PORTL |= 1 << PORTL2; // turn LED on
  } else {
    PORTL &= ~(1 << PORTL2); // turn LED off
  }
  if (x == 1000) { // to maintain a period of 1 second
    x = 0;
  }
  x++;
  return;
}

/**
 * task2 plays "Close Encounters of the Third Kind" theme and sleeps 
 * for 4 seconds until the theme plays again.
 *
 * The theme is played through a speaker connected to Pin 6.
 * task2 works by iterating through an array of values corresponding to OCRA4
 * values to output the frquencies corresponding to the theme.
 * 
 * Each note is played for 1 second and each pause for 2 seconds.
 * 
 * Then the speaker outputs nothing for 4 seconds until the theme repeats.
 * 
 * @param int on - turns task2 on
 */
void task2(int on) {
 if (on && !sleep) {
    OCR4A = melody[curr % 10]; // to play tone from melody array

    // to play tones for 1 sec
    if (timer % 1000 == 0 && !(melody[curr % 10] == NOTE_rest)) {
      curr++;
    }
    // to play paused for 2 seconds
    else if ((melody[curr % 10] == NOTE_rest) && (timer % 2000 == 0)) {
      curr++;
    }
    // to turn off the speaker for 4 seconds when theme is done
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
  // to turn speaker back on after 4 seconds
  if (sleep == 1 && timer % 6000 == 0) {
    sleep = 0;
  }
  return;
}

/**
 * task3 outputs the time since to program has started on 4 7-seg displays.
 * The number increases every 100ms.
 *
 * The number is output to the 7 seg display by iterating through
 * array seven_seg_digits and writes to the display through pins as such:
 * 
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
 * 
 * 
 * @param int on - turns task3 on
 */
void task3 (int on) {
  if (on) {
    if (timer % 100 == 0) counter++; // incriment counter every 100 ms

    // to show 100ms place on right seg 7
    if (timer % 4 == 0) {
      PORTC = 0b00000000;
      PORTA = 0b11111101;
      PORTC = seven_seg_digits[counter % 10];
    }
    // to show seconds place on second seg 7
    else if (timer % 4 == 1) {
      PORTC = 0b00000000;
      PORTA = 0b11111011;
      PORTC = seven_seg_digits[(counter / 10) % 10];
      PORTC |= 0b00000001;
    }
    // to show 10's seconds on third seg 7
    else if (timer % 4 == 2) {     
      PORTC = 0b00000000;
      PORTA = 0b11110111;
      PORTC = seven_seg_digits[(counter / 100) % 10];
    }
    // to show 100's seconds on 4th seg 7
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

/**
 * loop is to call tasks 1, 2, and 3 and then to incriment 
 * the global timer variable with a delay of 1 ms forever 
 * to complete all tasks according to Round Robin scheduler spec.
  */
void loop() {
    task1(1);
    task2(1);
    task3(1);
    timer++;
    delay(1);
}
