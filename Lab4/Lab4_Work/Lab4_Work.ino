/* Lab4_Work.ino
 * @file   Lab4_Work.ino
 *   @author    Ruvim Piholyuk, Illya Kuzmych
 *   @date      7-June-2022
 *   @brief   Lab 4 EE 474 Part I.3
 *   
 *  Lab4_Work.ino demonstrates our functioning freeRTOS program using Queues and 
 *  other freeRTOS function
 *  
 *  Utilize several functions to achieve desired functionality of part I.3 according to spec
 *  Pin 6 is for the speaker output to play Close Encounters, Pin 47 is for the LED
 *  
 *  Every task runs at the maximum tick rate allowed by freeRTOS (15 ms)
 *  The freeRTOS Queue is used to manage and send signals between the FFT. One is responsible for sending and receiving the 
 *  
 */

#include <arduinoFFT.h> ///< for the FFT algorithm
#include <Arduino_FreeRTOS.h> ///< for the freeRTOS functionality
#include <queue.h> ///< for the freeRTOS queue

#define NOTE_1 16000000 / (2 * 293) - 1    ///< to output 293 Hz to Pin 6
#define NOTE_2 16000000 / (2 * 329) - 1    ///< to output 329 Hz to Pin 6
#define NOTE_3 16000000 / (2 * 261) - 1    ///< to output 261 Hz to Pin 6
#define NOTE_4 16000000 / (2 * 130) - 1    ///< to output 130 Hz to Pin 6
#define NOTE_5 16000000 / (2 * 196) - 1    ///< to output 196 Hz to Pin 6
#define NOTE_rest 0                        ///< to output 0 Hz to Pin 6

#define ARR_SIZE 128 ///< buffer size to run the FFT


// define 5 tasks for Blink & Close Encounters ( RT1 & RT2 in the lab spec ) and for RT3, RT4
void RT1( void *pvParameters );
void RT2( void *pvParameters );
void RT3p0(void* parameter);
void RT3p1(void *pvParameters);
void RT4(void *pvParameters);

/// Task handle to turn off RT3p1
TaskHandle_t TaskRT3p1_Handler;


/// array defining all the frequencies of the melody
long melody[] = { NOTE_1, NOTE_rest, NOTE_2, NOTE_rest, NOTE_3, NOTE_rest, NOTE_4, NOTE_rest, NOTE_5 };
uint8_t curr = 0;
int timesRun = 0;

/// amplitude for the FFT to run with
int amplitude = 50;

/// for the FFT and Queue
QueueHandle_t xQueue1, xQueue2; ///< Queues for sending buffer data and "done computing" signal
arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
double vReal[ARR_SIZE]; ///< real portion of FFT
double vImag[ARR_SIZE]; ///< imaginary portion of the FFT
double p[ARR_SIZE]; ///< reference to buffer full of random doubles



// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);

  
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
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );
    
  xTaskCreate(
    RT2
    ,  "Close Encounters of the Third Kind"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    RT3p0
    ,  "RT3p0"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL ); 

  xTaskCreate(
    RT3p1
    ,  "RT3p1"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  &TaskRT3p1_Handler ); 

  xTaskCreate(
    RT4
    ,  "RT4"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );


    
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

/**
 * Turns on LED for 100 ms, off for 250 ms
 *
 * @param *pvParameters NULL parameter
 */
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


/**
 * Turns on Close Encounters with 1.5 second pause in between
 * through pin 6 and turns off after 3 play-throughs
 *
 * @param *pvParameters NULL parameter
 */
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
      vTaskSuspend(NULL); // suspend the current running task (RT2)
    }
  }
}





/**
 * Initializes two Queues for tasks RT3p1 and RT4
 * and fills up array with random double values
 *
 * @param *pvParameters NULL parameter
 */
void RT3p0(void *pvParameters) {
  vTaskSuspend(TaskRT3p1_Handler);
  double arrayOfData[ARR_SIZE];

  for (int i = 0; i < ARR_SIZE; i++) {
    arrayOfData[i] = (double) (random(5000)); 
  }
  
  for (int j = 0; j < ARR_SIZE; j++) {
    p[j] = arrayOfData[j];
  }
  xQueue1 = xQueueCreate(2, sizeof (double) );
  xQueue2 = xQueueCreate(2, sizeof (int) );


  vTaskResume(TaskRT3p1_Handler);
  vTaskSuspend(NULL);
  
}


/**
 * Sends the buffer to RT4 which computes the FFT.
 * Outputs the duration to compute the FFT 5 times
 *
 * @param *pvParameters NULL parameter
 */
void RT3p1(void *pvParameters) {
  for (;;) {
    int x, y, z, xy;
    
    x = millis();
    xQueueSendToBack(xQueue1, &p, portMAX_DELAY);
    Serial.println("Array of doubles now in the Queue1");
    xQueueReceive(xQueue2, &z, portMAX_DELAY);
    y = millis();

    xy = y - x;
    Serial.print("Time to run FFT (ms) : ");
    Serial.println(xy);
  }
}


/**
 * Receives buffer data through the Queue and computes FFT 5 times
 *
 * @param *pvParameters NULL parameter
 */
void RT4(void *pvParameters) {
  static double buffer1[ARR_SIZE];

  for (;;) {
    int x = 1;
    xQueueReceive(xQueue1, &buffer1, portMAX_DELAY);
    // Serial.println("Received data into buffer1");
    for (int i = 0; i < 5; i++) {
      // Serial.println("4 is running");
      for (int j = 0; j < ARR_SIZE; j++) {
        vReal[j] = buffer1[j]; // Build data with positive and negative values
        vImag[j] = 0.0; // imaginary part
      }
      FFT.Compute(vReal, vImag, ARR_SIZE, FFT_FORWARD); /* Compute FFT */
    }
    // Serial.println("FFT has run 5 times, now print time");
    xQueueSendToBack(xQueue2, &x, portMAX_DELAY); 
  }
}
