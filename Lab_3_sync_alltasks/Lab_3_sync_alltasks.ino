/* Lab_3_sync_alltasksS.ino
 * @file   Lab_3_sync_alltasksS.ino
 *   @author    Ruvim Piholyuk, Illya Kuzmych
 *   @date      26-May-2022
 *   @brief   Demo 4 according to spec
 *   
 *  Lab_3_sync_alltasksS.ino demonstrates both task 1, 2, and 3
 *  with the use of a SRRI scheduler.
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
 *  Pins 23-26 and Pins 30-37 control the 4 7-seg displays
 *  The time since the program has began is displayed on the 
 *  4 7-seg displays incrimenting every 100ms.
 *  the 4 displays display the time in 100 seconds, 10 seconds,
 *  seconds, and 100 ms from left to right.
 */
 
#define NOTE_1 16000000 / (2 * 293) - 1    ///< to output 293 Hz to Pin 6
#define NOTE_2 16000000 / (2 * 329) - 1    ///< to output 329 Hz to Pin 6
#define NOTE_3 16000000 / (2 * 261) - 1    ///< to output 261 Hz to Pin 6
#define NOTE_4 16000000 / (2 * 130) - 1    ///< to output 130 Hz to Pin 6
#define NOTE_5 16000000 / (2 * 196) - 1    ///< to output 196 Hz to Pin 6
#define NOTE_rest 0                        ///< to output 0 Hz to Pin 6

#define N_TASKS 5                          ///< Define for the total number of tasks in the scheduling array
#define READY 0                            ///< Define for ready state on of the tasks
#define RUNNING 1                          ///< Define for running state on of the tasks
#define SLEEPING 2                         ///< Define for sleeping state on of the tasks

#define PENDING 0                          ///< Define for ISR to indicate time has not incrimented
#define DONE 1                             ///< Define for ISR to indicate time has incrimented

/// For OCRA3 to overflow every 2 ms and to trigger the interupt
#define DELAY_TIMEOUT_VALUE 16000000 / (2 * 500) - 1


void (*taskPointers[N_TASKS]) (void);     ///< Array of function pointers for the scheduler
int state[N_TASKS];                       ///< Array to hold the state of the task at the corresponding index
int sleepingTime[N_TASKS];                ///< Array to hold the sleeping time of the task at the corresponding index
volatile int sFlag = PENDING;             ///< int that will be toggled to DONE when interupt is triggered

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
long melody[] = { NOTE_1, NOTE_rest, NOTE_2, NOTE_rest, NOTE_3, NOTE_rest, NOTE_4, NOTE_rest, NOTE_5 };

int curr = 0;                  ///< int to hold the index of the array accessing an index in the melody array
int task_index = 0;            ///< int to hold the index of the array accessing an index in the taskPointers array 
unsigned long timer = 0;       ///< int to hold the current time since the program has started
unsigned long counter = 0;     ///< int to hold the amount of times 100ms have passed

/**
 * setup to intialize ports to the desired specifications
 *
 * Initializing taskPointers to hold task1, task2, schedule_sync, and null in that order.
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
  taskPointers[0] = schedule_sync;
  taskPointers[1] = task1;
  taskPointers[2] = task2;
  taskPointers[3] = task3;
  taskPointers[4] = NULL;
  
  DDRA = 0b00011110;
  DDRC = 0xFF;
  DDRL |= 1 << DDL2;
  
  noInterrupts();
  TCCR4A = B01010100; ///< bottom two bits 0 (WGM31 & WGM30)
  TCCR4B = B00001001; ///< 4th bit set to 1 (WGMn4 & WGMn3) and set bottom bit for clock select

  /* for the delay timer */
  TCCR3A = B01010100; ///< bottom two bits 0 (WGMn1 & WGMn0)
  TCCR3B = B00001001; ///< 4th bit set to 1 (WGMn4 & WGMn3) and set bottom bit for clock select (prescaler of 1), clear timer on compare
  TIMSK3 = B00000010; ///< enable compare match interrupt for TIMER3
  OCR3A = DELAY_TIMEOUT_VALUE;
  
  DDRH |= 1 << DDH3; ///< pin 6
  interrupts();
  
}

/**
 * ISR to trigger when Counter 3 reaches OCRA3 value.
 * Changes sFlag to DONE to incriment global int timer variable
 */
ISR(TIMER3_COMPA_vect) {
    sFlag = DONE;
}

int x = 0;          ///< int to hold when Pin 6 will toggle on or off in task1

/**
 * task1 (void) toggles an LED at pin 6 on for 250 ms and 750 ms with a 1 second period
 *
 */
void task1(void) {
 if (state[task_index] != SLEEPING) {
    state[task_index] = RUNNING; 
    if (x < (250 / 2)) {
      PORTL |= 1 << PORTL2; // to turn LED on
    } else {
      PORTL &= ~(1 << PORTL2);   // to turn LED off
    }
    state[task_index] = READY;
 } else {
    state[task_index] = READY; // to ensure task 1 is never sleeping
 }
  if (x == (1000 / 2)) {
    x = 0;   // to keep the peiod of task1 as 1 second
  }
  x++;
  return;
}

/**
 * task2 (void) plays "Close Encounters of the Third Kind" theme and sleeps 
 * for 4 seconds until the theme plays again.
 *
 * The theme is played through a speaker connected to Pin 6.
 * task2 works by iterating through an array of values corresponding to OCRA4
 * values to output the frquencies corresponding to the theme.
 * 
 * Each note is played for 1 second and each pause for 2 seconds.
 * 
 * Then the speaker outputs nothing for 4 seconds until the theme repeats.
 */
void task2(void) {
 if (state[task_index] != SLEEPING) {
    state[task_index] = RUNNING;   
    OCR4A = melody[curr];
    // to play every note for 1 second
    if (timer % (1000 / 2) == 0 && (melody[curr] != NOTE_rest)) {
      curr++;
    }
    // to play every pause for 2 seconds
    else if ((melody[curr] == NOTE_rest) && (timer % (2000 / 2) == 0)) {
      curr++;
    }
    // to set task2 to sleep for 4 seconds once theme finishes playing
    if ( curr == 9 ) {
      sleep_474 (2000);
      curr = -1;
      OCR4A = 0;
    } else {
      state[task_index] = READY;
    }
  }
  else {
    OCR4A = 0;
    curr = -1;
  }
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
 */
void task3 (void) {
  if (state[task_index] != SLEEPING) {
    state[task_index] = RUNNING;
    // to incriment displayed value every 100ms
    if (timer % (100 / 2) == 0) counter++;

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
    state[task_index] = READY;
  } else {
    state[task_index] = READY;
    PORTA = 0b11111111;
  }
}



/**
 * start_function is a void function used to run the function passed into the function
 */
void start_function(void (*functionPtr) () ) {
  functionPtr(); // runs the function that was passed down
  return;
}

/**
 * sleep_474 is used to make the state index corresponding to the task
 * to SLEEPING and the sleepingTime array at the current index to @param t
 * @param param1 int t is used to hold the time a function to sleep in ms
 * 
 */
void sleep_474 (int t) {
  state[task_index] = SLEEPING; // change the state of the current task to sleep
  sleepingTime[task_index] = t;
  return;
}

/**
 * When sFLag is DONE:
 * 
 * schedule_sync is to decrement every index of sleepingTime
 * and to change states to READY of any index sleepingTime is zero.
 * 
 * Also, timer is incrimented
 * 
 * sFlag is set to PENDING again
 * 
 */
void schedule_sync(void) {
  if (sFlag != PENDING) {
    timer++;
    // to decriment sleepTime if need be
    for (int k = 0; k < N_TASKS; k++) {
      if (sleepingTime[k] > 0) {
        sleepingTime[k] -= 1;
      }
      // to set tasks to READY if need be
      if (sleepingTime[k] <= 0) {
        state[k] = READY;
      }
    }
  }
  sFlag = PENDING;
  return;
}

/**
* scheduler iterates through taskPointers to complete every task.
* Until scheduler has reached the end of taskPointers where is begins 
* at the start of taskPointer again.
 */
void scheduler(void) {
  if ((taskPointers[task_index] == NULL) && (task_index != 0)) {
    task_index = 0;
  }
  if (task_index > (N_TASKS - 1)) { // just in case above one fails
    task_index = 0;
  }
  start_function(taskPointers[task_index]);
  task_index++;
  return;
}

/**
 * loop is to continually calling the SRRI scheduler when sFlag is DONE.
 * SFlag is set to PENDING once scheduler is complete so no tasks are being done
 * until 2 ms have passed.
 */
void loop() {
  if (sFlag == DONE) {
    scheduler();
    sFlag = PENDING; // reset the interrupt flag
  }
}
