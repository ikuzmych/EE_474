#include <Arduino_FreeRTOS.h>

#define ECHOPIN 10
#define TRIGPIN 9
#define INPUT_PORT_B4 (~(1 << DDB4))
#define OUTPUT_PORT_H6 (1 << DDH6)
#define OUTPUT_PORT_L2 (1 << DDL2)
#define OUTPUT_PORT_L4 (1 << DDL4)


#define TURN_OFF_LED (~(1 << PORTL2))
#define TURN_ON_LED (1 << PORTL2)
#define TURN_ON_BUZZER (1 << PORTL4)
#define TURN_OFF_BUZZER (~(1 << PORTL4))


/* Constants for LED and buzzer rates */
#define SIX_FEET 72
#define FIVE_FEET 60
#define FOUR_FEET 48 
#define THREE_FEET 36
#define TWO_FEET 24
#define ONE_FEET 12



long duration;
int distance;


void setup() {
  Serial.begin(9600);

  DDRL |= OUTPUT_PORT_L2;
  DDRH |= OUTPUT_PORT_H6;
  DDRB &= INPUT_PORT_B4;
  DDRL |= OUTPUT_PORT_L4;

  xTaskCreate(
    readData
    ,  "Read the Sensor Data"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    ledWarning
    ,  "Adjustor for LED blink rate"   // A name just for humans
    ,  512  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    buzzerWarning
    ,  "Buzzer sound"   // A name just for humans
    ,  512  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );
    
}

void loop() {
  // does nothing 
}

void readData(void *pvParameters) {
  for (;;) {
    PORTH &= ~(1 << PORTH6);
    vTaskDelay( 1 / portTICK_PERIOD_MS );
    PORTH |= 1 << PORTH6;
    vTaskDelay(5 / portTICK_PERIOD_MS );
    PORTH &= ~(1 << PORTH6);
  
    duration = pulseIn(ECHOPIN, HIGH);
    distance = (duration * 0.034 / 2) / 2.54; // distance in inches
    Serial.print("Distance: ");
    Serial.println(distance); 
  }
}

void ledWarning(void *pvParameters) {
  int delayTime;
  
  for (;;) {
    if (distance > SIX_FEET) {
      delayTime = 10000;
    } 
    
    else if (distance > FIVE_FEET) { ///< between 5 and 6 feet
      delayTime = 1500;
    } 
    
    else if (distance > FOUR_FEET) { ///< between 5 and 6 feet
      delayTime = 1250;
    } 
    
    else if (distance > THREE_FEET) { ///< between 5 and 6 feet
      delayTime = 1000;
    } 
    
    else if (distance > TWO_FEET) { ///< between 5 and 6 feet
      delayTime = 750;
    } 
    
    else if (distance > ONE_FEET) { ///< between 5 and 6 feet
      delayTime = 250;
    } 
    
    else {
      delayTime = 100;
    }

    if (delayTime > 2000) {
      PORTL &= TURN_OFF_LED;
    } else {
      PORTL |= TURN_ON_LED;
      vTaskDelay(delayTime / portTICK_PERIOD_MS); // on for 2 secs, off for 2 secs
    }

    if (distance > SIX_FEET) {
      delayTime = 10000;
    } 
    
    else if (distance > FIVE_FEET) { ///< between 5 and 6 feet
      delayTime = 1500;
    } 
    
    else if (distance > FOUR_FEET) { ///< between 5 and 6 feet
      delayTime = 1250;
    } 
    
    else if (distance > THREE_FEET) { ///< between 5 and 6 feet
      delayTime = 1000;
    } 
    
    else if (distance > TWO_FEET) { ///< between 5 and 6 feet
      delayTime = 750;
    } 
    
    else if (distance > ONE_FEET) { ///< between 5 and 6 feet
      delayTime = 250;
    } 
    
    else {
      delayTime = 100;
    }

    if (delayTime > 2000) {
      PORTL &= TURN_OFF_LED;
    } else {
      PORTL &= TURN_OFF_LED;
      vTaskDelay(delayTime / portTICK_PERIOD_MS);
    }
  }
}

void buzzerWarning(void *pvParameters) {
  int delayTime;
  
  for (;;) {
    if (distance > THREE_FEET) {
      delayTime = 10000;
    }
    
    else if (distance > TWO_FEET) { ///< between 5 and 6 feet
      delayTime = 750;
    } 
    
    else if (distance > ONE_FEET) { ///< between 5 and 6 feet
      delayTime = 250;
    }
    
    else {
      delayTime = 100;
    }

    if (delayTime > 750) {
      PORTL &= TURN_OFF_BUZZER;
    } else {
      PORTL |= TURN_ON_BUZZER;
      vTaskDelay(delayTime / portTICK_PERIOD_MS); // on for 2 secs, off for 2 secs
    }
    
    if (distance > THREE_FEET) {
      delayTime = 10000;
    }
    
    else if (distance > TWO_FEET) { ///< between 5 and 6 feet
      delayTime = 750;
    } 
    
    else if (distance > ONE_FEET) { ///< between 5 and 6 feet
      delayTime = 250;
    }
    
    else {
      delayTime = 100;
    }
    
      if (delayTime > 750) {
      PORTL &= TURN_OFF_BUZZER;
    } else {
      PORTL &= TURN_OFF_BUZZER;
      vTaskDelay(delayTime / portTICK_PERIOD_MS);
  }
}
}
