void setup() {
  // initialize pin outputs.

  // DDRL |= 1 << DDL2;
  // DDRL |= 1 << DDL1;
  // DDRL |= 1 << DDL0;

  noInterrupts();
  TCCR4A = B01010100; // bottom two bits 0 (WGMn1 & WGMn0)
  TCCR4B = B00001001; // 4th bit set to 1 (WGMn4 & WGMn3) and set bottom bit for clock select
  DDRH |= 1 << DDH3; // pin 6

  
  
  /* pinMode(47, OUTPUT);
     pinMode(48, OUTPUT);
     pinMode(49, OUTPUT);
  */
}

/* task 1 in lab2 
 *  Simple light pattern utilizing digitalWrite()
 */
//void lab2task1() {
//    digitalWrite(47, HIGH);
//    digitalWrite(49, LOW);
//    delay(333);
//    digitalWrite(47, LOW);
//    digitalWrite(48, HIGH);
//    delay(333);
//    digitalWrite(48, LOW);
//    digitalWrite(49, HIGH);
//    delay(333);
//}

/* task2 in lab2 
 *  Utilizes PORTL and bit-shifting
 *  to turn on and off the LEDs in the right
 *  order
 */
//void lab2task2() {
//    PORTL |= 1 << PORTL2;
//    PORTL &= ~(1 << PORTL0);
//    delay(333);
//    PORTL &= ~(1 << PORTL2);
//    PORTL |= 1 << PORTL1;
//    delay(333);
//    PORTL &= ~(1 << PORTL1);
//    PORTL |= 1 << PORT0;
//    delay(333);
//}


void lab2task3() {
  static int time = 0;
  if (time <= 1000) OCR4A = 16000000 / 400 - 1;
  else if (time <= 2000) OCR4A = 16000000 / 250 - 1;
  else if (time <= 3000) OCR4A = 16000000 / 800 - 1;
  else if (time <= 4000) OCR4A = 0;
  else if (time == 4001) time = 0;
  
  time++;
  delay(1);  
}



// the loop function runs over and over again forever
void loop() {
  // lab2task1();   // something else dumb that ruvim made
  // lab2task2();   // something dumb ruvim made
  // lab2task3()   // sound pattern
}
