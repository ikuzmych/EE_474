///*
// * Written by Ishaan Bhimani 
// *
// */

#define OP_DECODEMODE  8
#define OP_SCANLIMIT   10
#define OP_SHUTDOWN    11
#define OP_DISPLAYTEST 14
#define OP_INTENSITY   10

//Transfers 1 SPI command to LED Matrix for given row
//Input: row - row in LED matrix
//       data - bit representation of LEDs in a given row; 1 indicates ON, 0 indicates OFF
void spiTransfer(volatile byte row, volatile byte data);
unsigned long Time = 0;
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
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(2, INPUT_PULLUP);
  /* pin 47 */
  DDRL |= 1 << DDL2;
  /* pin 49 */
  DDRL |= 1 << DDL0;
  /* pin 51 */
  DDRB |= 1 << DDB2;
  /* set pin 49 to high */
  PORTL |= 1 << PORTL0;

  spiTransfer(OP_DISPLAYTEST,0);
  spiTransfer(OP_SCANLIMIT,7);
  spiTransfer(OP_DECODEMODE,0);
  spiTransfer(OP_SHUTDOWN,1);
}

void loop(){ 
//  xPosition = analogRead(A0);
//  yPosition = analogRead(A1);
//  SW_state = digitalRead(2);
//  Serial.print("X: ");
//  Serial.println(xPosition);
//  Serial.print("Y: ");
//  Serial.println(yPosition);
//  delay(500);
//  int j = 0;
//  int i = 0;
//  // for (j = 0; j < 8; j++){ //for each row, set the LEDs
//    // spiTransfer(j, 0b10100101);
//    spiTransfer(3, 0b00100000);
//    delay(50);
//    spiTransfer(3, 0b00000000);
//    spiTransfer(4, 0b00100000);
//    
//  // }
//  delay(500);
//  for (i = 0; i < 8; i++){ //for each row, clear the LEDs
//    spiTransfer(i, 0b00000000);
//  }
//  delay(500);
  joyStickGame(analogRead(A0),analogRead(A1));
  
}


void joyStickGame(int xPos, int yPos) {
  calcX = (int) xPos / 145;
//  calcY = pow(2, (yPos / 128)) + 1;
  calcY = (int) yPos / 145;
  spiTransfer(currX, currY);
  if (Time % 25 == 0) {
    spiTransfer(currX, 0b00000000);
    currX = calcX;
    currY = 0b00000001  << calcY;
    Serial.print("X: ");
    Serial.println(currX);
    Serial.print("Y: ");
    Serial.println(currY);
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
