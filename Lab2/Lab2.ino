#define NOTE_c 3830 // 261 Hz
#define NOTE_d 3400 // 294 Hz
#define NOTE_e 3038 // 329 Hz
#define NOTE_f 2864 // 349 Hz
#define NOTE_g 2550 // 392 Hz
#define NOTE_a 2272 // 440 Hz
#define NOTE_b 2028 // 493 Hz
#define NOTE_C 1912 // 523 Hz
#define NOTE_R 0

/* for mary had a little lamb  */
int melody[] = { NOTE_e, NOTE_R, NOTE_d, NOTE_R, NOTE_c, NOTE_R, NOTE_d, NOTE_R, NOTE_e, NOTE_R,NOTE_e, NOTE_R,NOTE_e, NOTE_R,NOTE_d, NOTE_R,NOTE_d, NOTE_R,NOTE_d, 
                 NOTE_R,NOTE_e, NOTE_R,NOTE_g, NOTE_R,NOTE_g, NOTE_R,NOTE_e, NOTE_R,NOTE_d, NOTE_R,NOTE_c, NOTE_R,NOTE_d, NOTE_R,NOTE_e, 
                 NOTE_R,NOTE_e, NOTE_R,NOTE_e, NOTE_R,NOTE_e, NOTE_R,NOTE_d, NOTE_R,NOTE_d, NOTE_R,NOTE_e, NOTE_R,NOTE_d, NOTE_R,NOTE_c, NOTE_R,NOTE_c };
unsigned long curr = 0;
unsigned long Time = 0;
int taskA = 0;
int taskB = 0;
int taskMary = 0;
int s_A;
int s_B;
void setup() {
  // initialize pin outputs.

   DDRL |= 1 << DDL2;
   DDRL |= 1 << DDL1;
   DDRL |= 1 << DDL0;

  noInterrupts();
  TCCR4A = B01010100; // bottom two bits 0 (WGMn1 & WGMn0)
  TCCR4B = B00001001; // 4th bit set to 1 (WGMn4 & WGMn3) and set bottom bit for clock select
  DDRH |= 1 << DDH3; // pin 6
  interrupts();
  
     /*
     pinMode(47, OUTPUT);
     pinMode(48, OUTPUT);
     pinMode(49, OUTPUT);
    */
}

/* task 1 in lab2 
 *  Simple light pattern utilizing digitalWrite()
 */
 /*
void lab2task1() {
    digitalWrite(47, HIGH);
    digitalWrite(49, LOW);
    delay(333);
    digitalWrite(47, LOW);
    digitalWrite(48, HIGH);
    delay(333);
    digitalWrite(48, LOW);
    digitalWrite(49, HIGH);
    delay(333);
}

/* task2 in lab2 
 *  Utilizes PORTL and bit-shifting
 *  to turn on and off the LEDs in the right
 *  order
 */
void task_A() {
    PORTL |= 1 << PORTL2;
    PORTL &= ~(1 << PORTL0);
    delay(333);
    PORTL &= ~(1 << PORTL2);
    PORTL |= 1 << PORTL1;
    delay(333);
    PORTL &= ~(1 << PORTL1);
    PORTL |= 1 << PORT0;
    delay(333);
    return;
}

void task_A1(int on) {
  if (on) {
    if ((s_A != 2) && (Time % 333 == 0)) {
      s_A++;
    } else if (Time % 333 == 0) {
      s_A = 0;
    }
    if (s_A == 2) {
      PORTL &= ~(1 << PORTL2);
      PORTL &= ~(1 << PORTL1);
      PORTL |= 1 << PORT0;
    } else if (s_A == 1) {
      PORTL &= ~(1 << PORTL2);
      PORTL &= ~(1 << PORTL0);
      PORTL |= 1 << PORT1;
    } else if (s_A == 0) {
      PORTL &= ~(1 << PORTL1);
      PORTL &= ~(1 << PORTL0);
      PORTL |= 1 << PORT2;
    }
  } else {
      PORTL &= ~(1 << PORTL2);
      PORTL &= ~(1 << PORTL1);
      PORTL &= ~(1 << PORTL0);
  }
}


//void task_B() {
//  static int timeB = 0;
//  if (timeB <= 1000) OCR4A = 16000000 / 400 - 1;
//  else if (timeB <= 2000) OCR4A = 16000000 / 250 - 1;
//  else if (timeB <= 3000) OCR4A = 16000000 / 800 - 1;
//  else if (timeB <= 4000) OCR4A = 0;
//  else if (timeB == 4001) timeB = 0;
//  
//  timeB++;
//  delay(1);  
//}
void task_B() {
  OCR4A = 16000000 / 400 - 1;
  delay(1000);
  OCR4A = 16000000 / 250 - 1;
  delay(1000);
  OCR4A = 16000000 / 800 - 1;
  delay(1000);
  OCR4A = 0;
  delay(1000);
  return;
}

void task_B1 (int on) {
  if (on) {
    if ((s_B != 3) && (Time % 1000 == 0)) {
      s_B++;
    } else if (Time % 1000 == 0) {
      s_B = 0;
    }
    if (s_B == 0) OCR4A = 16000000 / 400 - 1;
    else if (s_B == 1) OCR4A = 16000000 / 250 - 1;
    else if (s_B == 2) OCR4A = 16000000 / 800 - 1;
    else if (s_B == 3) OCR4A = 0;
  } else {
    OCR4A = 0;
  }
}



//void task_C() {
//  
//  PORTL |= 1 << PORTL2;
//  PORTL &= ~(1 << PORTL0);
//  delay(333);
//  PORTL &= ~(1 << PORTL2);
//  PORTL |= 1 << PORTL1;
//  delay(333);
//  PORTL &= ~(1 << PORTL1);
//  PORTL |= 1 << PORT0;
//  delay(333);
//  PORTL |= 1 << PORTL2;
//  PORTL &= ~(1 << PORTL0);
//  delay(333);
//  PORTL &= ~(1 << PORTL2);
//  PORTL |= 1 << PORTL1;
//  delay(333);
//  PORTL &= ~(1 << PORTL1);
//  PORTL |= 1 << PORT0;
//  delay(333);
//  PORTL &= ~(1 << PORTL0);
//  PORTL &= ~(1 << PORTL1);
//  PORTL &= ~(1 << PORTL2);
//
//  OCR4A = 16000000 / 400 - 1;
//  delay(1000);
//  OCR4A = 16000000 / 250 - 1;
//  delay(1000);
//  OCR4A = 16000000 / 800 - 1;
//  delay(1000);
//  OCR4A = 0;
//  delay(2000);
//  
//}
void task_C() {
  task_A();
  task_A();
  PORTL &= ~(1 << PORTL0);
  PORTL &= ~(1 << PORTL1);
  PORTL &= ~(1 << PORTL2);
  task_B();
  OCR4A = 0;
  delay(1000);
}

void task_C1 () {
  task_A1 (taskA);
  // task_B1 (taskB);
  task_mhall(taskMary);
  if (Time == 1) {
    taskA = 1;
    // taskB = 0;
    taskMary = 0;
  } else if (Time == 2000) {
    taskA = 0;
    // taskB = 1;
    taskMary = 1;
  } else if (Time == 18750) {
    taskA = 1;
    // taskB = 1;
    taskMary = 1;
  } if (Time == 28750) {
    taskA = 0;
    // taskB = 0;
    taskMary = 0;
  }
  if (Time == 29750) {
    Time = 0;
  }
  Time++;
  delay(1);
}


void task_mhall(int on) {
  if (on) {
    OCR4A = melody[curr % 53];
    if (Time % 500 == 0) {
      curr++;
    }
    else if ((melody[curr % 53] == NOTE_R) && (Time % 125 == 0)) {
      curr++;
    }
  }
  else {
    OCR4A = 0;
    curr = 0;
  }
  
}



// the loop function runs over and over again forever
void loop() {
  // lab2task1();
  // task_A();   // task A go
  // task_B();   // task B go
  // task_C();
  task_C1();
}
