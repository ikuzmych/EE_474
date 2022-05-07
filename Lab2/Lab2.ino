void setup() {
  // initialize pin outputs.

  DDRL |= 1 << DDL2;
  DDRL |= 1 << DDL1;
  DDRL |= 1 << DDL0;
  
  /* pinMode(47, OUTPUT);
     pinMode(48, OUTPUT);
     pinMode(49, OUTPUT);
  */
}

/* task 1 in lab2 
 *  Simple light pattern utilizing digitalWrite()
 */
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
void lab2task2() {
    PORTL |= 1 << PORTL2;
    PORTL &= ~(1 << PORTL0);
    delay(333);
    PORTL &= ~(1 << PORTL2);
    PORTL |= 1 << PORTL1;
    delay(333);
    PORTL &= ~(1 << PORTL1);
    PORTL |= 1 << PORT0;
    delay(333);
}


// the loop function runs over and over again forever
void loop() {
  lab2task1();
  lab2task2();
}
