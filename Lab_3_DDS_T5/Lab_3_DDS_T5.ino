/* Lab_3_DDS_T5.ino
 * @file   Lab_3_DDS_T5.ino
 *   @author    Ruvim Piholyuk, Illya Kuzmych
 *   @date      26-May-2022
 *   @brief   Demo 6 according to spec
 *   
 *  Lab_3_DDS_T5.ino demonstrates both task 1, 2, and 3
 *  with the use of a DDS scheduler.
 *  
 * Task 1 runs all the time.
 * Task 2 runs at the start but stops after playing the theme 2 times. 
 * After Task 2, start a count-down (on the 7-segment display) for 3 seconds and 
 * then restarts task 2 (original definition above) for one final time.  
 * Display a “smile” for 2 seconds (See Appendix B) (code this as a new task to 
 * be controlled by Task 5).   The 2 second interval begins after completion of part 5c.
 * Stop all tasks, except "Task 1” in this list. 

 *  Pin 47 is used to toggle an LED on for 250 ms 
 *  and off for 750ms repeating every second.
 *  
 *  Pin 6 is used to play the theme of "Close Encounters
 *  of the Third Kind" through a speaker. Every note is played
 *  for one second and there is a 2 second pause between notes.
 *  Once all notes are played the task sleeps for 4 seconds 
 *  until it repeats. 
 *  
 *  Pins 23-26 and Pins 30-37 control the 4 7-seg display.
 *  4 7-seg displays change every 100ms.
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
#define DEAD 3                             ///< Define for dead state on of the tasks

#define PENDING 0                          ///< Define for ISR to indicate time has not incrimented
#define DONE 1                             ///< Define for ISR to indicate time has incrimented

/// For OCRA3 to overflow every 2 ms and to trigger the interupt
#define DELAY_TIMEOUT_VALUE 16000000 / (4 * 500) - 1

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

int curr = -1;                  ///< int to hold the index of the array accessing an index in the melody array
int task_index = 0;             ///< int to hold the index of the array accessing an index in the taskPointers array 
unsigned long timer = 0;        ///< int to hold the current time since the program has started
unsigned long counter = 30;     ///< int to hold the amount of times 100ms have passed


/**
 * struct DDS hold data corresponding to each task
 *  according to the desired specifications.
 *
 * a void pointer to the a specific task
 * a 20 character string to hold a specific name
 * int state to hold the current state of the task
 * unsigned int sleepTime to hold how much longer for the task to sleep
 * unsigned int timesStarted to hold how many times each task has been started
 */
typedef struct DDS {
  void (*id)(void);
  char taskName [20];
  int state;
  unsigned int sleepTime;
  unsigned int timesStarted;
} DDS;

/// TASKS is an array of DDS structs holding data according to the specifications
DDS TASKS [N_TASKS];

/**
 * setup to intialize ports to the desired specifications
 *
 * Initializing indexes of TASKS according to tsk 1, 2, 3, 
 * schedule_sync, and null
 * 
 * Set Pin 6 to output
 * Set pin 47 to output
 * 
 * Set Counter 4 to no prescaler and to reset when overflowed
 * Set Counter 3 to count for 2 ms
 *
 */
void setup() {
  TASKS[0].id = task1;
  strcpy(TASKS[0].taskName, "Blinking Light     ");
  TASKS[0].state = READY;
  TASKS[0].sleepTime = 0;
  TASKS[0].timesStarted = 0;  
  
  TASKS[1].id = task2;
  strcpy(TASKS[1].taskName, "Close Encounters     ");
  TASKS[1].state = READY;
  TASKS[1].sleepTime = 0;
  TASKS[1].timesStarted = 0;

  TASKS[2].id = task3;
  strcpy(TASKS[2].taskName, "Countdown Timer      ");
  TASKS[2].state = READY;
  TASKS[2].sleepTime = 0;
  TASKS[2].timesStarted = 0;

  TASKS[3].id = schedule_sync;
  strcpy(TASKS[3].taskName, "Schedule Sync      ");
  TASKS[3].state = READY;
  TASKS[3].sleepTime = 0;
  TASKS[3].timesStarted = 0;

  TASKS[4].id = NULL;
  
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
 * task1 runs all the time
 */
void task1(void) {
 if (TASKS[task_index].state == READY) {
    TASKS[task_index].state = RUNNING; 
    if (x < (250 / 2)) {
      PORTL |= 1 << PORTL2; // to turn LED on
    } else {
      PORTL &= ~(1 << PORTL2);  // to turn LED off  
    }
    TASKS[task_index].state = READY;
 } else {
    TASKS[task_index].state = READY;  // for task1 to never sleep
 }
  if (x == (1000 / 2)) {
    x = 0; // to keep a period of 1s on task 1
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
 * 
 * Task 2 works until the theme is played twice then it is dead for 3 seconds
 * and plays the theme one final time before turning off completely.
 */
void task2(void) {
 if (TASKS[task_index].state == READY) {
    TASKS[task_index].state = RUNNING;   
    OCR4A = melody[curr];
    TASKS[task_index + 1].state = SLEEPING;
    // to play each tone for 1 second
    if (timer % (1000 / 2) == 0 && (melody[curr] != NOTE_rest)) {
      curr++;
    }
    // to play each pause for 2 seconds
    else if ((melody[curr] == NOTE_rest) && (timer % (2000 / 2) == 0)) {
      curr++;
    }
    // to put task to sleep if it has played 1
    // to turn off task if is has been played 2 or more times
    if ( curr == 9 ) {
      TASKS[task_index].timesStarted += 1;
      curr = -1;
      OCR4A = 0;
      counter = 30;
      if ( TASKS[task_index].timesStarted < 2) {
        sleep_474(2000);
      }
      else if (TASKS[task_index].timesStarted == 2) {
             counter = 30;
             task_self_quit();
      } else if (TASKS[task_index].timesStarted > 2) {
          task_self_quit();
          task_start(&TASKS[2], task3);  // to turn task 3 from DEAD to READY
          counter = 21;
      } 
    } else {
      TASKS[task_index].state = READY;
    }
  }
  else {
    OCR4A = 0;
    curr = -1;
  }
  return;
}

/**
 * task3 outputs the countdown until task 2 is on after the second time the theme is played on 4 7-seg displays.
 * 
 * then it is turned off and displays a smile for 2 seconds before turning off forever.
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
  if ((TASKS[task_index].state == READY) && (TASKS[1].timesStarted == 2)) {
    TASKS[task_index].state = RUNNING;
    
    // to count down for 3 seconds after task 2 has played twice
    if (timer % (100 / 2) == 0) counter--;

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
    // to turn task 3 to DEAD when count down is at zero
    // and start task2 again
    if (counter == 0) {
      task_start(&TASKS[1], task2); // start task 2
      task_self_quit();   
      TASKS[task_index].timesStarted++;
      counter = 21;
    } else {
       TASKS[task_index].state = READY;
    } 
  } else if ((TASKS[task_index].state == READY) && (TASKS[1].timesStarted == 3)){
    // to turn off the Seg 7 after 2 seconds
    if (counter == 0) {
      PORTC = 0b00000000;
      PORTA = 0b11100001;
      task_self_quit();
    }
    // to count down from 2 seconds
    if (timer % (100 / 2) == 0) {
      counter--;
    }

    // to display the smile
    if (timer % 4 == 0) {
      PORTC = 0b00000000;
      PORTA = 0b11111101;
      PORTC = 0b01001010; // first part of smiley face (right most)
    }
    else if (timer % 4 == 1) {
      PORTC = 0b00000000;
      PORTA = 0b11111011;
      PORTC = 0b10010000;
    }
    else if (timer % 4 == 2) {     
      PORTC = 0b00000000;
      PORTA = 0b11110111;
      PORTC = 0b10010000;
    }
    else if (timer % 4 == 3) {
      PORTC = 0b00000000;
      PORTA = 0b11101111;
      PORTC = 0b00100110;
    }
    
  } 
  else {
    PORTC = 0b00000000;
    PORTA = 0b00011110;
  }
  return;
}

/**
 * start_function is a void function used to run the function passed into the function
 * 
 * @param void (*functionPtr) gives the function a pointer to start the given task
 */
void start_function(void (*functionPtr) () ) {
  functionPtr(); // runs the function that was passed down
  return;
}

/**
 * task_start is a void function used to run the task passed into the function
 * @param DDS *task is the DDS struct of the task to be started
 * @ param void (*functionPtr) is the task desired to be started
 */
void task_start(DDS *task, void (*functionPtr) () ) {
//  task->id = functionPtr;
  task->state = READY;
  return;
}

/**
 * task_self_quit is a void function used to
 * force a task "dead" for the remainder of the run time
 */
void task_self_quit(void) {
//  TASKS[task_index].id = NULL;
  TASKS[task_index].state = DEAD;
  return;
}

/**
 * sleep_474 is used to make the state index corresponding to the task
 * to SLEEPING and the sleepingTime array at the current index to @param t
 * 
 * @param param1 int t is used to hold the time a function to sleep in ms
 */
void sleep_474 (int t) {
  TASKS[task_index].state = SLEEPING; // change the state of the current task to sleep
  TASKS[task_index].sleepTime = t;
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
    timer++; // to incriment global "timer"

    // to decriment sleepTime if need be
    for (int k = 0; k < N_TASKS; k++) {
      if (TASKS[k].sleepTime > 0) {
        TASKS[k].sleepTime -= 1;
      }
      // to set tasks to READY if need be
      if (TASKS[k].sleepTime <= 0 && TASKS[k].state == SLEEPING) {
        TASKS[k].state = READY;
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
  if ((TASKS[task_index].id == NULL) && (task_index >= N_TASKS - 1)) {
    task_index = 0;
  }
  if (TASKS[task_index].id == NULL) {
    task_index++;
  }
  start_function(TASKS[task_index].id);
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
