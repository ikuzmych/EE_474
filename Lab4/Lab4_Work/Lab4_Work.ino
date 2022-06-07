#include <arduinoFFT.h>
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
void RT3p0(void* parameter);
void RT3p1(void *pvParameters);
void RT4(void *pvParameters);

TaskHandle_t TaskRT3p1_Handler;


/// array defining all the frequencies of the melody
long melody[] = { NOTE_1, NOTE_rest, NOTE_2, NOTE_rest, NOTE_3, NOTE_rest, NOTE_4, NOTE_rest, NOTE_5 };
uint8_t curr = 0;
int timesRun = 0;
int N = 64;

int amplitude = 50;
// int* nPointer = &N;



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
      vTaskSuspend(NULL);
    }
  }
}



QueueHandle_t xQueue1, xQueue2;
arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
double vReal[64];
double vImag[64];
double p[64];


void RT3p0(void *pvParameters) {
  vTaskSuspend(TaskRT3p1_Handler);
  double arrayOfData[64];

  for (int i = 0; i < 64; i++) {
    arrayOfData[i] = (double) (random(5000)); 
  }
  
  for (int j = 0; j < 64; j++) {
    p[j] = arrayOfData[j];
  }
  xQueue1 = xQueueCreate(2, sizeof (double) );
  xQueue2 = xQueueCreate(2, sizeof (int) );


  vTaskResume(TaskRT3p1_Handler);
  vTaskSuspend(NULL);
  
}


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



void RT4(void *pvParameters) {
  static double buffer1[64];

  for (;;) {
    int x = 1;
    xQueueReceive(xQueue1, &buffer1, portMAX_DELAY);
    // Serial.println("Received data into buffer1");
    for (int i = 0; i < 5; i++) {
      // Serial.println("4 is running");
      for (int j = 0; j < 64; j++) {
        vReal[j] = buffer1[j]; // Build data with positive and negative values
        vImag[j] = 0.0; // imaginary part
      }
      FFT.Compute(vReal, vImag, 64, FFT_FORWARD); /* Compute FFT */
    }
    // Serial.println("FFT has run 5 times, now print time");
    xQueueSendToBack(xQueue2, &x, portMAX_DELAY); 
  }
}
