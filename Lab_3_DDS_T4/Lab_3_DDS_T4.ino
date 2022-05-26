#define NOTE_1 16000000 / (2 * 293) - 1
#define NOTE_2 16000000 / (2 * 329) - 1
#define NOTE_3 16000000 / (2 * 261) - 1
#define NOTE_4 16000000 / (2 * 130) - 1
#define NOTE_5 16000000 / (2 * 196) - 1
#define NOTE_rest 0
#define freq1 293
#define freq2 329
#define freq3 261
#define freq4 130
#define freq5 196
#define freqR 0


#define N_TASKS 4
#define READY 0
#define RUNNING 1
#define SLEEPING 2
#define DEAD 3

#define PENDING 0
#define DONE 1

#define DELAY_TIMEOUT_VALUE 16000000 / (2 * 500) - 1


void (*taskPointers[N_TASKS]) (void);
int state[N_TASKS];
int sleepingTime[N_TASKS]; // to keep track of how long each function needs to sleep, will constantly decrement
volatile int sFlag = PENDING;
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

int frequencies[9] =  { freq1, freqR, freq2, freqR, freq3, freqR, freq4, freqR, freq5 };                              
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
long melody[] = { NOTE_1, NOTE_rest, NOTE_2, NOTE_rest, NOTE_3, NOTE_rest, NOTE_4, NOTE_rest, NOTE_5 };
int curr = -1;
int task_index = 0;
// int sleep;
unsigned long timer = 0;
unsigned long counter = 40;

typedef struct DDS {
  void (*id)(void);
  char taskName [20];
  int state;
  unsigned int sleepTime;
  unsigned int timesStarted;
} DDS;

DDS TASKS [N_TASKS];

void setup() {
  TASKS[0].id = task2;
  strcpy(TASKS[0].taskName, "Close Encounters     ");
  TASKS[0].state = READY;
  TASKS[0].sleepTime = 0;
  TASKS[0].timesStarted = 0;

  TASKS[1].id = task3;
  strcpy(TASKS[1].taskName, "Countdown Timer      ");
  TASKS[1].state = READY;
  TASKS[1].sleepTime = 0;
  TASKS[1].timesStarted = 0;

  TASKS[2].id = schedule_sync;
  strcpy(TASKS[2].taskName, "Schedule Sync      ");
  TASKS[2].state = READY;
  TASKS[2].sleepTime = 0;
  TASKS[2].timesStarted = 0;

  TASKS[3].id = NULL;
  
  DDRA = 0b00011110;
  DDRC = 0xFF;
  DDRL |= 1 << DDL2;
  
  noInterrupts();
  TCCR4A = B01010100; // bottom two bits 0 (WGM31 & WGM30)
  TCCR4B = B00001001; // 4th bit set to 1 (WGMn4 & WGMn3) and set bottom bit for clock select

  /* for the delay timer */
  TCCR3A = B01010100; // bottom two bits 0 (WGMn1 & WGMn0)
  TCCR3B = B00001001; // 4th bit set to 1 (WGMn4 & WGMn3) and set bottom bit for clock select (prescaler of 1), clear timer on compare
  TIMSK3 = B00000010; // enable compare match interrupt for TIMER3
  OCR3A = DELAY_TIMEOUT_VALUE;
  
  DDRH |= 1 << DDH3; // pin 6
  interrupts();
  
}

ISR(TIMER3_COMPA_vect) {
    sFlag = DONE;
}


/**
 * Playing a song for Task 2...
 */
void task2(void) {
 if (TASKS[task_index].state == READY) {
    TASKS[task_index].state = RUNNING;   
    OCR4A = melody[curr];
    TASKS[task_index + 1].state = SLEEPING;
    if (timer % (1000 / 2) == 0 && (melody[curr] != NOTE_rest)) {
      curr++;
    }
    else if ((melody[curr] == NOTE_rest) && (timer % (2000 / 2) == 0)) {
      curr++;
    }
    if ( curr == 9 ) {
      sleep_474 (2000);
      curr = -1;
      OCR4A = 0;
      TASKS[task_index + 1].state = READY;
      counter = 40;
    } else {
      TASKS[task_index].state = READY;
    }
    if (timer % 4 == 0) {
      PORTC = 0b00000000;
      PORTA = 0b11111101;
      PORTC = seven_seg_digits[(frequencies[curr]) % 10];
    }
    else if (timer % 4 == 1) {
      PORTC = 0b00000000;
      PORTA = 0b11111011;
      PORTC = seven_seg_digits[(frequencies[curr] / 10) % 10];
    }
    else if (timer % 4 == 2) {     
      PORTC = 0b00000000;
      PORTA = 0b11110111;
      PORTC = seven_seg_digits[(frequencies[curr] / 100) % 10];
    }
    else if (timer % 4 == 3) {
      PORTC = 0b00000000;
      PORTA = 0b11101111;
      PORTC = seven_seg_digits[(frequencies[curr] / 1000) % 10];
    }
  }
  else {
    OCR4A = 0;
    curr = -1;
  }
}

void task3 (void) {
  if (TASKS[task_index].state != SLEEPING) {
    TASKS[task_index].state = RUNNING;
    if (timer % (100 / 2) == 0) counter--;
    
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
    TASKS[task_index].state = READY;
  } else {
  //  TASKS[task_index].state = READY;
    PORTA = 0b11111111;
  }
}


void start_function(void (*functionPtr) () ) {
  functionPtr(); // runs the function that was passed down
  return;
}
 
void sleep_474 (int t) {
  TASKS[task_index].state = SLEEPING; // change the state of the current task to sleep
  TASKS[task_index].sleepTime = t;
  return;
}


void schedule_sync(void) {

  if (sFlag != PENDING) {
    timer++;
    for (int k = 0; k < N_TASKS; k++) {
      if (TASKS[k].sleepTime > 0) {
        TASKS[k].sleepTime -= 1;
      }
      if (TASKS[k].sleepTime <= 0) {
        TASKS[k].state = READY;
      }
    }
  }
  sFlag = PENDING;
  return;
}


void scheduler(void) {
  if ((TASKS[task_index].id == NULL) && (task_index != 0)) {
    task_index = 0;
  }
  if (task_index > (N_TASKS - 1)) { // just in case above one fails
    task_index = 0;
  }

  start_function(TASKS[task_index].id);
  task_index++;
  return;
}

void loop() {
  if (sFlag == DONE) {
    scheduler();
    sFlag = PENDING; // reset the interrupt flag
  }
}