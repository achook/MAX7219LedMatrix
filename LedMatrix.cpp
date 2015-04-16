
#include <SPI.h>
#include "LedMatrix.h"
#include "cp437font.h"

/**
*  numberOfDevices: how many modules are daisy changed togehter
*  slaveSelectPin: which pin is controlling the CS/SS pin of the first module?
*/
void LedMatrix::init(byte numberOfDevices, byte slaveSelectPin) {
  myNumberOfDevices = numberOfDevices;
  mySlaveSelectPin = slaveSelectPin;
  
  pinMode(mySlaveSelectPin, OUTPUT);
  
  SPI.begin ();
  for(byte device = 0; device < myNumberOfDevices; device++) {
    sendByte (device, MAX7219_REG_SCANLIMIT, 7);   // show all 8 digits
    sendByte (device, MAX7219_REG_DECODEMODE, 0);  // using an led matrix (not digits)
    sendByte (device, MAX7219_REG_DISPLAYTEST, 0); // no display test
    sendByte (device, MAX7219_REG_INTENSITY, 0);  // character intensity: range: 0 to 15
    sendByte (device, MAX7219_REG_SHUTDOWN, 1);   // not in shutdown mode (ie. start it up)  
  }
}


void LedMatrix::sendByte (const byte device, const byte reg, const byte data) {  
  int offset=device;
  int maxbytes=myNumberOfDevices;

  for(int i=0;i<maxbytes;i++) {
    spidata[i] = (byte)0;
    spiregister[i] = (byte)0;
  }
  //put our device data into the array
  spiregister[offset] = reg;
  spidata[offset] = data;
  //enable the line 
  digitalWrite(mySlaveSelectPin,LOW);
  //Now shift out the data 
  for(int i=0;i<myNumberOfDevices;i++) {
    SPI.transfer (spiregister[i]);
    SPI.transfer (spidata[i]);      
  }
  digitalWrite (mySlaveSelectPin, HIGH); 


}  // end of sendByte
 
 

void LedMatrix::clear() {
    for (byte col = 0; col < myNumberOfDevices * 8; col++) {
      cols[col] = 0;
    }

} 

void LedMatrix::commit() {
  for (byte col = 0; col < myNumberOfDevices * 8; col++) {
    sendByte(col / 8, col % 8 + 1, cols[col]);
  }
}

void LedMatrix::setText(String text) {
  myText = text;
  myTextOffset = 0;
}

void LedMatrix::scrollTextRight() {
   myTextOffset = (myTextOffset + 1) % ((int)myText.length() * 7 - 5); 
}
void LedMatrix::scrollTextLeft() {
   myTextOffset = (myTextOffset - 1) % ((int)myText.length() * 7 - 5); 
}

void LedMatrix::oscillateText() {
  int maxColumns = (int)myText.length() * 7;
  int maxDisplayColumns = myNumberOfDevices * 8;
  if (maxDisplayColumns > maxColumns) {
    return;
  }
  if (myTextOffset - maxDisplayColumns == -maxColumns) {
    increment = 1;
  }
  if (myTextOffset == 0) {
    increment = -1;
  }
  myTextOffset += increment;
}

void LedMatrix::drawText() {
  char letter;
  int position = 0;
  for (int i = 0; i < myText.length(); i++) {
    letter = myText.charAt(i);
    for (byte col = 0; col < 8; col++) {
      position = i * 7 + col + myTextOffset;
      if (position >= 0 && position < myNumberOfDevices * 8) {
        setColumn(position, pgm_read_byte (&cp437_font [letter] [col]));
      }
    }  
  }  
}

void LedMatrix::setColumn(int column, byte value) {
  if (column < 0 || column >= myNumberOfDevices * 8) {
    return;
  }
  cols[column] = value;
}

void LedMatrix::setPixel(byte x, byte y) {
  bitWrite(cols[x], y, true);   
}

