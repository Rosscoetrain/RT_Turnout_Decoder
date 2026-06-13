 /*
 *  © 2023,2026 Ross Scanlon
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this code.  If not, see <https://www.gnu.org/licenses/>.
*/

// This is a DCC Accessory Decoder to drive 8 Pulsed Turnouts
// Based on the NMRA Pulsed 8 stationary decoder


#include "defines.h"

// need EEPROM if using I2C control for i2c address storage
#if defined (STM32F1xx_Blue_Pill) || defined (STM32F1xx_Stumpy)

// Use 0-2. Larger for more debugging messages
#define FLASH_DEBUG      2

// You can select another sector. Be careful not larger than (REGISTERED_NUMBER_FLASH_SECTORS - 1) and large enough not to overwrite your program
// Default is (REGISTERED_NUMBER_FLASH_SECTORS - 1) if you don't specify here
#define USING_FLASH_SECTOR_NUMBER           (REGISTERED_NUMBER_FLASH_SECTORS - 2)

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "FlashStorage_STM32F1.h"

#include <Wire.h>
#endif


#include <NmraDcc.h>
#include "PinPulser.h"

/*
 * Rosscoe Train functions and variables
 */

#include "version.h"

#include "variables.h"

#include "functions.h"
#include "functions_dcc.h"

#if defined (STM32F1xx_Blue_Pill) || defined (STM32F1xx_Stumpy)
#include "EXIO/i2c_functions.h"
#endif


void setup()
 {
#ifdef ENABLE_SERIAL
  MYSERIAL.begin(115200);
  uint8_t maxWaitLoops = 255;
  while(!MYSERIAL && maxWaitLoops--)
    delay(20);
#endif

  setVersion();

  // on STM32F1xx boards check if we are connecting via I2C or DCC
  // DCC i2cControl = FALSE
  // I2C i2cControl = TRUE
  // set jumper at PB4 on these boards.

#if defined (STM32F1xx_Blue_Pill) || defined (STM32F1xx_Stumpy)
  disableJTAG();
  pinMode(I2C_CONTROL, INPUT_PULLUP);
//  i2cControl = digitalRead(I2C_CONTROL);
  i2cControl = false;
//  EEPROM.init();
#endif

  // Setup which External Interrupt, the Pin it's associated with that we're using and enable the Pull-Up
  // Many Arduino Cores now support the digitalPinToInterrupt() function that makes it easier to figure out the
  // Interrupt Number for the Arduino Pin number, which reduces confusion.

  if (!i2cControl)
   {
#ifdef digitalPinToInterrupt
    Dcc.pin(DCC_PIN, 0);
#else
    Dcc.pin(0, DCC_PIN, 1);
#endif

#ifdef ENABLE_DCC_ACK
    pinMode(ENABLE_DCC_ACK, OUTPUT);
#endif

#ifdef LEARNING
    pinMode(LEARNINGBUTTON, INPUT);
#endif

// Call the main DCC Init function to enable the DCC Receiver
    Dcc.init( MAN_ID_DIY, DCC_DECODER_VERSION_NUM, FLAGS_OUTPUT_ADDRESS_MODE | FLAGS_DCC_ACCESSORY_DECODER, 0 );
   }

#if defined (STM32F1xx_Blue_Pill) || defined (STM32F1xx_Stumpy)
  else
   {
// setup I2C control
    if (getI2CAddress() != 0)
     {
      i2cAddress = getI2CAddress();
     }
    if (i2cAddress < 0x08 || i2cAddress > 0x77)
     {
      MYSERIAL.print(F("ERROR: Invalid I2C address configured: 0x"));
      MYSERIAL.print(i2cAddress, HEX);
      MYSERIAL.println(F(", using default instead"));
      i2cAddress = I2C_ADDRESS;
     }
    MYSERIAL.print(F("Available at I2C address 0x"));
    MYSERIAL.println(i2cAddress, HEX);

    Wire.begin(i2cAddress);
// disable I2C pullups internally
    digitalWrite(I2C_SDA, LOW);
    digitalWrite(I2C_SCL, LOW);

    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);

    


   }
#endif  // STM32F1xx


#ifdef ENABLE_SERIAL
#ifdef KATO_SMT_BOARD
  MYSERIAL.print("Rosscoe Train DCC 8 Turnout Accessory Decoder Kato. ");
#else
  MYSERIAL.print("Rosscoe Train DCC 8 Turnout Accessory Decoder. ");
#endif
  MYSERIAL.print(F("Version: "));
  MYSERIAL.print(versionBuffer[0]);
  MYSERIAL.print(F("."));
  MYSERIAL.print(versionBuffer[1]);
  MYSERIAL.print(F("."));
  MYSERIAL.println(versionBuffer[2]);
  MYSERIAL.println();
#endif

#ifdef FORCE_RESET_FACTORY_DEFAULT_CV
#ifdef ENABLE_SERIAL
  MYSERIAL.println("Resetting CVs to Factory Defaults");
  notifyCVResetFactoryDefault();
#endif
#endif

  if( FactoryDefaultCVIndex == 0)	// Not forcing a reset CV Reset to Factory Defaults so initPinPulser
   {
	  initPinPulser();
   }
}

void loop()
{
#ifdef LEARNING
  static int learningbuttonOldval = 0,learningbuttonVal = 0;
#endif

  // You MUST call the NmraDcc.process() method frequently from the Arduino loop() function for correct library operation
  Dcc.process();

  pinPulser.process();

  if( FactoryDefaultCVIndex && Dcc.isSetCVReady())
   {
    FactoryDefaultCVIndex--; // Decrement first as initially it is the size of the array
    uint16_t cv = FactoryDefaultCVs[FactoryDefaultCVIndex].CV;
    uint8_t val = FactoryDefaultCVs[FactoryDefaultCVIndex].Value;
#ifdef DEBUG_MSG
    MYSERIAL.print("loop: Write Default CV: "); MYSERIAL.print(cv,DEC); MYSERIAL.print(" Value: "); MYSERIAL.println(val,DEC);
#endif
    Dcc.setCV( cv, val );

    if( FactoryDefaultCVIndex == 0)	// Is this the last Default CV to set? if so re-initPinPulser
	    initPinPulser();
   }


  ////////////////////////////////////////////////////////////////
  // check if the learning button has been enabled
  //
  // This is the jumper on A6 marked Learn on the PCB.
  // jumper must be in place to enter learning mode
  // When in learning mode send a close or throw command from the command station
  // and the address will be programmed into the decoder
  //
  ////////////////////////////////////////////////////////////////

#ifdef LEARNING

#ifdef STM32F1xx
  learningbuttonVal = digitalRead(LEARNINGBUTTON);
#else
  learningbuttonVal = dr(LEARNINGBUTTON);
#endif

  if (learningbuttonOldval != learningbuttonVal)
   {
    learningMode = learningbuttonVal;
    if (learningMode == HIGH) showAcknowledge(3);
   }
  learningbuttonOldval = learningbuttonVal;
#endif

#ifdef ENABLE_SERIAL
    // see if there are serial commands
  readString="";              //empty for next input

  while (MYSERIAL.available())
   {
    char c = MYSERIAL.read();     //gets one byte from serial buffer
    readString += c;            //makes the string readString
    delay(10);                   //slow looping to allow buffer to fill with next character
   }
  // act on serial commands

  if (readString.length() >0)
   {
    doSerialCommand(readString);
   }
#endif

}




