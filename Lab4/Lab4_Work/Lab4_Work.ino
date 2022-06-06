#include <Arduino_FreeRTOS.h>
#include <queue.h> 

#define NOTE_1 16000000 / (2 * 293) - 1    ///< to output 293 Hz to Pin 6
#define NOTE_2 16000000 / (2 * 329) - 1    ///< to output 329 Hz to Pin 6
#define NOTE_3 16000000 / (2 * 261) - 1    ///< to output 261 Hz to Pin 6
#define NOTE_4 16000000 / (2 * 130) - 1    ///< to output 130 Hz to Pin 6
#define NOTE_5 16000000 / (2 * 196) - 1    ///< to output 196 Hz to Pin 6
#define NOTE_rest 0                        ///< to output 0 Hz to Pin 6

// define 3 tasks for Blink & Close Encounters ( RT1 & RT2 in the lab spec ) and for RT3
void RT1( void *pvParameters );
void RT2( void *pvParameters );
void RT3p0(void *pvParameters);
void RT3p1(void *pvParameters);

TaskHandle_t TaskRT2_Handler;
TaskHandle_t TaskRT3p0_Handler;
TaskHandle_t TaskRT3p1_Handler;


/// array defining all the frequencies of the melody
long melody[] = { NOTE_1, NOTE_rest, NOTE_2, NOTE_rest, NOTE_3, NOTE_rest, NOTE_4, NOTE_rest, NOTE_5 };
uint8_t curr = 0;
int timesRun = 0;
int N = 20;
int* nPointer = &N;



// the setup function runs once when you press reset or power the board
void setup() {

  noInterrupts();
    TCCR4A = B01010100; ///< bottom two bits 0 (WGM31 & WGM30)
    TCCR4B = B00001001; ///< 4th bit set to 1 (WGMn4 & WGMn3) and set bottom bit for clock select
  interrupts();
  DDRH |= 1 << DDH3; ///< pin 6
  DDRL |= 1 << DDL2; ///< pin 47
  
  xTaskCreate(
    RT1
    ,  "Blink"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );
    
  xTaskCreate(
    RT2
    ,  "Close Encounters of the Third Kind"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  &TaskRT2_Handler );
  
  xTaskCreate(
    RT3p0
    ,  "RT3p0"   // A name just for humans
    ,  256  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  nPointer
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  &TaskRT3p0_Handler ); 


  xTaskCreate(
    RT3p1
    ,  "RT3p1"   // A name just for humans
    ,  256  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  &TaskRT3p1_Handler ); 


    
  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
  //  (note how the above comment is WRONG!!!)
  vTaskStartScheduler();
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void RT1(void *pvParameters)  // This is a task.
{
 // (void) pvParameters;  // allocate stack space for params

  for (;;) // A Task shall never return or exit.
  {
    PORTL |= 1 << PORTL2;   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for one second
    PORTL &= ~(1 << PORTL2);    // turn the LED off by making the voltage LOW
    vTaskDelay( 250 / portTICK_PERIOD_MS ); // wait for one second
  }
}

void RT2(void *pvParameters)  // This is a task.
{
 // (void) pvParameters;  // allocate stack space for params

  // initialize digital LED_BUILTIN on pin 13 as an output.

  for (;;) // A Task shall never return or exit.
  {
    OCR4A = melody[curr];
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
    curr++;
    if (curr == 9 && (timesRun < 2)) {
      timesRun++;
      OCR4A = 0;
      curr = 0;
      vTaskDelay( 1500 / portTICK_PERIOD_MS ); // wait for 1.5 second
    } else if (curr == 9) {
      OCR4A = 0;
      vTaskSuspend(TaskRT2_Handler);
    }
  }
}



QueueHandle_t xQueue;

void RT3p0(void *pvParameters) {
  
  int arraysize = &pvParameters;

  double arrayOfData[arraysize];

  for (int i = 0; i < arraysize; i++) {
    arrayOfData[i] = (double) (random(0, 5000)); 
  }
  xQueue = xQueueCreate(arraysize, sizeof (double) );
  vTaskSuspend(TaskRT3p0_Handler);
  vTaskResume(TaskRT3p1_Handler);
}

void RT3p1(void *pvParameters) {

}
