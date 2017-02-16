#include <SPI.h>



using namespace std;
//*****if the write was successful, but carried over to next chip do not send anything
//only return 1 if there is not more memory availibe to write to
//arduino compiler does not support exceptions

//not sure how to make it one continuous block of memory
//not sure when to return 2
//how will we know what we are reading
// if char or c string, etc
//what is write protection and do i need it
//what is hold pin


// function to check how much space there is left?


#define MOSI 51
#define MISO 50
#define SCK 7
//write protect 49
//hold 48
#define DENSITY 16000000

#define SS1 43s
#define SS2 42
#define SS3 41
#define SS4 40
bool ss1Writable = true;
bool ss2Writable = true;
bool ss3Writable = true;
bool ss4Writable = true;
int lastSS = 0;
#define READ
#define WRITE
int32_t spaceLeft[4] = {DENSITY, DENSITY, DENSITY, DENSITY}; //16MB

#define MAXSPEED 50000000

SPISettings testSettings = SPISettings(13000000, MSBFIRST, SPI_MODE0);

void setup() {
  pinMode(MOSI, OUTPUT); //MOSI MISO might be defaulted
  pinMode(MISO, INPUT);
  pinMode(SS1, INPUT);
  pinMode(SS2, INPUT);
  pinMode(SS3, INPUT);
  pinMode(SS4, INPUT);

  digitalWrite(SS1, HIGH);
  digitalWrite(SS2, HIGH);
  digitalWrite(SS3, HIGH);
  digitalWrite(SS4, HIGH);
  //used to turn off file transfers LOW --> active

  SPI.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}


/*
  int memWrite(address, char[]);
  Writes c string char to memory address address. Returns 0 if successful.
  Returns 1 if attempting to write past the end of memory.
  Returns 2 if EEPROM fails to ack, potential hardware failure.
*/
int memWrite(int address, char data[]) {

  int SS = whichEEPROM();
  if (SS == -1) {
    return 1;
  }
  digitalWrite(SS, LOW);
  SPI.beginTransaction(testSettings);
  for (int i = 0; i < strlen(data); i++) {
    shiftOut(SS, SCK, MSBFIRST, data[i]);
  }
  digitalWrite(SS, HIGH);
  return 0;

  //shiftOut(SS, SCK, MSBFIRST, data)

}
/*
  int memWriteInt(address, int);
  Writes 1 byte integer value int to memory address address.
  Returns 0 if successful. Returns 1 if attempting to write past the end of memory.
  Returns 2 if EEPROM fails to act, potential hardware failure.
*/
int memWriteInt(int address, int data) {
  int SS = whichEEPROM();
  if (SS == -1) {
    return 1;
  }
  digitalWrite(SS, LOW);
  SPI.beginTransaction(testSettings);
  byte bytedata[4];
  bytedata[0] = (byte) (data & 0xFF);
  bytedata[1] = (byte) ((data >> 8) & 0xFF);
  bytedata[2] = (byte) ((data >> 8) & 0xFF);
  bytedata[3] = (byte) ((data >> 8) & 0xFF);
  for (int i = 0; i < sizeof(bytedata); i++) {
    shiftOut(SS, SCK, MSBFIRST, bytedata[i]);
  }
  //possible access through memory address
  digitalWrite(SS, HIGH);
  return 0;
}


/*
  int memWriteLong(address, long);
  Writes 4 byte integer value long to memory address address.
  Returns 0 if successful. Returns 1 if attempting to write past the end of memory.
  Returns 2 if EEPROM fails to ack, potential hardware failure.
*/
int memWriteLong(int address, long data) { // 4 bytes
  int SS = whichEEPROM();
  if (SS == -1) {
    return 1;
  }
  digitalWrite(SS, LOW);
  SPI.beginTransaction(testSettings); //this is sort of unlcear why it is needed
  byte bytedata[4];
  bytedata[0] = (byte) (data & 0xFF);
  bytedata[1] = (byte) ((data >> 8) & 0xFF);
  bytedata[2] = (byte) ((data >> 8) & 0xFF);
  bytedata[3] = (byte) ((data >> 8) & 0xFF);
  for (int i = 0; i < 4; i++) {

    shiftOut(SS, SCK, MSBFIRST, bytedata[i]);
  }
  digitalWrite(SS, HIGH);
  //possible access through memory address
  return 0;

}
/*
  int memRead(address, char[], length);
  Read length characters into character array char[].
  Returns 0 if successful. Returns 1 if attempting to read past the end of memory.
  Returns 2 if EEPROM fails to ack, potential hardware failure.
  Will not check that length is greater than allocated length of char[].
*/
int memRead(char* address, char arr[], int length) {

  int SS = whichEEPROM();
  if (SS == -1) {
    return 1;
  }
  digitalWrite(SS, LOW);
  SPI.beginTransaction(testSettings);
  for (int i = 0; i < length; i++) {
    SPI.transfer(arr[i]);
  }
  digitalWrite(SS, HIGH);
  return 0;
}
/*
  int memWriteInt(address, *int);
  Read 1 byte integer value into value at integer pointer int.
  Returns 0 if successful. Returns 1 if attempting to read past the end of memory.
  Returns 2 if EEPROM fails to ack, potential hardware failure.
*/
int memWriteInt(int* address, int* data) {
  byte intByte;

}

/*
  int memWriteLong(address, *long);
  Read 4 byte integer value into value at long pointer long.
  Returns 0 if successful. Returns 1 if attempting to read past the end of memory.
  Returns 2 if EEPROM fails to ack, potential hardware failure.
*/
int memWriteLong(long* address, long* data) {

}

int whichEEPROM() { //determines which eeprom is availible to write
  if (ss1Writable) { //returns -1 if there are no more available ones --> cause error leading to catch condition
    return SS1;
  }
  else if (ss2Writable) {
    return SS2;
  }
  else if (ss3Writable) {
    return SS3;
  }
  else if (ss4Writable) {
    return SS4;
  }
  else return -1;
}

void full(int fullSlavePin) {
  switch (fullSlavePin) {
    case SS1:
      ss1Writable = false;
      break;
    case SS2:
      ss2Writable = false;
      break;
    case SS3:
      ss3Writable = false;
      break;
    case SS4:
      ss4Writable = false;
      break;
    default:
      break;
  }
}

