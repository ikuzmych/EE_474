///*
// * Written by Ishaan Bhimani 
// *
// */

#define OP_DECODEMODE  8
#define OP_SCANLIMIT   10
#define OP_SHUTDOWN    11
#define OP_DISPLAYTEST 14
#define OP_INTENSITY   10

/* Notes for the mary had a little lamb task */
#define NOTE_c 3830 // 261 Hz
#define NOTE_d 3400 // 294 Hz
#define NOTE_e 3038 // 329 Hz
#define NOTE_f 2864 // 349 Hz
#define NOTE_g 2550 // 392 Hz
#define NOTE_a 2272 // 440 Hz
#define NOTE_b 2028 // 493 Hz
#define NOTE_C 1912 // 523 Hz
#define NOTE_R 0


//Transfers 1 SPI command to LED Matrix for given row
//Input: row - row in LED matrix
//       data - bit representation of LEDs in a given row; 1 indicates ON, 0 indicates OFF
void spiTransfer(volatile byte row, volatile byte data);
unsigned long Time = 0;
int melody[] = { NOTE_e, NOTE_R, NOTE_d, NOTE_R, NOTE_c, NOTE_R, NOTE_d, NOTE_R, NOTE_e, NOTE_R,NOTE_e, NOTE_R,NOTE_e, NOTE_R,NOTE_d, NOTE_R,NOTE_d, NOTE_R,NOTE_d, 
                 NOTE_R,NOTE_e, NOTE_R,NOTE_g, NOTE_R,NOTE_g, NOTE_R,NOTE_e, NOTE_R,NOTE_d, NOTE_R,NOTE_c, NOTE_R,NOTE_d, NOTE_R,NOTE_e, 
                 NOTE_R,NOTE_e, NOTE_R,NOTE_e, NOTE_R,NOTE_e, NOTE_R,NOTE_d, NOTE_R,NOTE_d, NOTE_R,NOTE_e, NOTE_R,NOTE_d, NOTE_R,NOTE_c, NOTE_R,NOTE_c };

unsigned long curr = 0;
// change these pins as necessary
int DIN = 47;
int CS =  49;
int CLK = 51;

byte spidata[2]; //spi shift register uses 16 bits, 8 for ctrl and 8 for data

int xPosition = 0;
int yPosition = 0;
int SW_state = 0;

int calcX, calcY, currX, currY;



void setup(){
  Serial.begin(9600);
  // pinMode(A0, INPUT);
  // pinMode(A1, INPUT);
 //  pinMode(2, INPUT_PULLUP);
  /* pin 47 */
  DDRL |= 1 << DDL2;
  /* pin 49 */
  DDRL |= 1 << DDL0;
  /* pin 51 */
  DDRB |= 1 << DDB2;
  /* set pin 49 to high */
  PORTL |= 1 << PORTL0;


  DDRK &= ~(1 << DDK0);
  DDRK &= ~(1 << DDK1);
  
  noInterrupts();
  TCCR4A = B01010100; // bottom two bits 0 (WGMn1 & WGMn0)
  TCCR4B = B00001001; // 4th bit set to 1 (WGMn4 & WGMn3) and set bottom bit for clock select
  DDRH |= 1 << DDH3; // pin 6
  interrupts();

  spiTransfer(OP_DISPLAYTEST,0);
  spiTransfer(OP_SCANLIMIT,7);
  spiTransfer(OP_DECODEMODE,0);
  spiTransfer(OP_SHUTDOWN,1);
}

void loop(){
  joyStickGame(analogRead(A0),analogRead(A1), 1);
}


void joyStickGame(int xPos, int yPos, int on) {
  if (on) {
    calcX = (int) xPos / 145;
    calcY = (int) yPos / 145;
    spiTransfer(currX, currY);
    if (Time % 50 == 0) {
      spiTransfer(currX, 0b00000000);
      currX = calcX;
      currY = 0b00000001  << calcY;
    }
  } else {
    for (int i = 0; i < 8; i++) {
      spiTransfer(i, 0b00000000);
    }
  }
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

  Time++;
  delay(1);
}


void spiTransfer(volatile byte opcode, volatile byte data){
  int offset = 0; //only 1 device
  int maxbytes = 2; //16 bits per SPI command
  
  for(int i = 0; i < maxbytes; i++) { //zero out spi data
    spidata[i] = (byte)0;
  }
  //load in spi data
  spidata[offset+1] = opcode+1;
  spidata[offset] = data;
  digitalWrite(CS, LOW); //
  for(int i=maxbytes;i>0;i--)
    shiftOut(DIN,CLK,MSBFIRST,spidata[i-1]); //shift out 1 byte of data starting with leftmost bit
  digitalWrite(CS,HIGH);
}
