/*
 * @file variables.h
 */

/*
 *  © 2023 Ross Scanlon
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



#ifndef VARIABLES_H
#define VARIABLES_H

struct CVPair
{
  uint16_t  CV;
  uint8_t   Value;
};


/*
 * to use individual CV's for each address output requires two CV's
 * one for the pulse time
 * one for the output state
 *
 */


/*
 *           address
 *           1    2    3    4    5    6    7    8
 * CV
 *  pulse   33   35   37   39   41   43   45   47
 *  state   34   36   38   40   42   44   46   48
 */


/*
 * These are stored in an array of CV pairs.
 * Default array is as below
 *
 */



CVPair FactoryDefaultCVs [] =
{
  {CV_ACCESSORY_DECODER_ADDRESS_LSB, DEFAULT_ACCESSORY_DECODER_ADDRESS & 0xFF},
  {CV_ACCESSORY_DECODER_ADDRESS_MSB, DEFAULT_ACCESSORY_DECODER_ADDRESS >> 8},
  {CV_MANUFACTURER_ID, 13},
  {CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME, DEFAULT_RECHARGE},   // x 10mS for the CDU recharge delay time
  {CV_29_CONFIG, CV29_ACCESSORY_DECODER},


#ifdef SINGLE_PULSE
  {CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME, 50},   // x 10mS for the output pulse duration
  {CV_ACCESSORY_DECODER_ACTIVE_STATE,    ACTIVE_OUTPUT_STATE},
#else
  {CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME, 10},   // x this for the output pulse duration
  {33, DEFAULT_PULSE},
  {34, ACTIVE_OUTPUT_STATE},
  {35, DEFAULT_PULSE},
  {36, ACTIVE_OUTPUT_STATE},
  {37, DEFAULT_PULSE},
  {38, ACTIVE_OUTPUT_STATE},
  {39, DEFAULT_PULSE},
  {40, ACTIVE_OUTPUT_STATE},
  {41, DEFAULT_PULSE},
  {42, ACTIVE_OUTPUT_STATE},
  {43, DEFAULT_PULSE},
  {44, ACTIVE_OUTPUT_STATE},
  {45, DEFAULT_PULSE},
  {46, ACTIVE_OUTPUT_STATE},
  {47, DEFAULT_PULSE},
  {48, ACTIVE_OUTPUT_STATE},
#endif

  {CV_ACCESSORY_DECODER_SERIAL_MSB, uint8_t(SERIAL_NUMBER / 256)},
  {CV_ACCESSORY_DECODER_SERIAL_LSB, uint8_t(SERIAL_NUMBER - ((SERIAL_NUMBER / 256) * 256))},


};



uint8_t FactoryDefaultCVIndex = 0;

// This is the Arduino Pin Mapping to Turnout Addresses with 2 pins per turnout
// A1 is missing in the sequence as it is used for the DCC ACK
// The Pins are defined in Pairs T=Thrown, C=Closed (Digitrax Notation)
//   base address 1C 1T 2C 2T 3C 3T  4C 4T  5C  5T  6C  6T  7C  7T  8C  8T
#ifdef NANO_SMT_BOARD

//   base address 1C 1T 2C 2T 3C 3T  4C 4T  5C  5T  6C  6T  7C  7T  8C  8T
byte outputs[] = { 4, 3, 6, 5, 8, 7, 10, 9, 11, 12, 14, 13, 17, 16, 19, 18};
//   pins         D4 D3 D6 D5 D8 D7 D10 D9 D11 D12  A0 D13  A3  A2  A5  A4

#endif

#ifdef KATO_SMT_BOARD

#warning "Build for Kato"

//   base address 1C 1T 2C 2T 3C 3T  4C  4T  5C 5T  6C  6T  7C  7T  8C  8T
byte outputs[] = { 3, 4, 5, 6, 7, 8, 12, 11, 10, 9, 13, 14, 16, 17, 18, 19};
//   pins         D3 D4 D5 D6 D7 D8 D12 D11 D10 D9 D13  A0  A2  A3  A4  A5

#endif


//#elif defined(ARDUINO_ARCH_ESP32)

//#ifndef ESP32_38PIN
//#warning "Build for ESP32"
// this is the ESP32-WROOM-32 Pin Mapping to Turnout Addresses

//   base address  1C  1T  2C  2T  3C  3T  4C  4T  5C  5T  6C  6T  7C  7T  8C  8T
//byte outputs[] = { 33, 32, 26, 25, 14, 27, 13, 12, 15,  4, 17, 16, 18,  5, 21, 19};

//#elif defined(ESP32_38PIN)

//#warning "Building for ESP32 38pin board on nano carrier"

// this is the ESP32-WROOM-32E Pin Mapping to Turnout Addresses when used on nano carrier board

//   base address  1C  1T  2C  2T  3C  3T  4C  4T  5C  5T  6C  6T  7C  7T  8C  8T
//byte outputs[] = {  4, 34, 35,  5, 33, 32, 26, 25, 27, 12, 14, 13, 17, 16, 19, 18};
//#endif


//#else

//   base address 1C 1T 2C 2T 3C 3T  4C 4T  5C  5T  6C  6T  7C  7T  8C  8T
//byte outputs[] = { 4, 3, 6, 5, 8, 7, 10, 9, 12, 11, 14, 13, 17, 16, 19, 18};
//   pins         D4 D3 D6 D5 D8 D7 D10 D9 D12 D11  A0 D13  A3  A2  A5  A4

//#endif



#ifdef STM32F1xx_Blue_Pill
//   base address  1C   1T   2C   2T   3C   3T   4C   4T   5C   5T   6C   6T   7C   7T   8C   8T
byte outputs[] = { PC13,PC14,PC15,PA0, PA1, PA2, PA4, PA3, PA6, PA5, PB0, PA7, PB10,PB1, PB9, PB11};
//   pins          PC13 PC14 PC15 PA0  PA1  PA2  PA4  PA3  PA6  PA5  PB0  PA7  PB10 PB1  PB9  PB11 
//                 32   33   34   0    1    2    4    3    6    5   16    7    26  17   25    27
//   net           AD3  AD4  AD5  AD6  AD7  AD8  AD10 AD9  AD12 AD11 AD14 AD13 AD16 AD15 AD18 AD17

uint16_t vpins[] = {   800,      801,      802,      803,      804,      805,      806,      807 };

//#define I2C_CONTROL PB4
//#define I2C_SDA PB7
//#define I2C_SCL PB6
#endif


#ifdef STM32F1xx_Stumpy
//   base address  1C   1T   2C   2T   3C   3T   4C   4T   5C   5T   6C   6T   7C   7T   8C   8T
byte outputs[] = { PB0, PB1, PA5, PA7, PA3, PA1, PA2, PA0, PA6, PA4, PB11,PA2, PB5, PB15,PB9, PB3};
//   pins          PB0  PB1  PA5  PA7  PA3  PA1  PA2  PA0  PA6  PA4  PB11 PA2  PB5  PB15 PA9  PB3 
//                 16   17   5    7    3    1    2    0    6    4   27    2             
//   net           AD3  AD4  AD5  AD6  AD7  AD8  AD10 AD9  AD12 AD11 AD14 AD13 AD16 AD15 AD18 AD17
uint16_t vpins[] = {   800,      801,      802,      803,      804,      805,      806,      807 };

//#define I2C_CONTROL PB4
//#define I2C_SDA PB7
//#define I2C_SCL PB6
#endif


/*
STM32F103C8T6 Bluepill EX-IOExpander pin map at Vpin 800


Vpin  Pin   Digital  Analogue  PWM       Output
800   PC13  Y        N         N
801   PC14  Y        N         N
802   PA15  Y        N         N
803   PA0   Y        Y         N
804   PA1   Y        Y         Y
805   PA2   Y        Y         Y
806   PA3   Y        Y         Y
807   PA4   Y        Y         N
808   PA5   Y        Y         N
809   PA6   Y        Y         Y
810   PA7   Y        Y         Y
811   PB0   Y        Y         Y
812   PB1   Y        Y         Y
813   PB10  Y        N         Y
814   PB11  Y        N         Y
815   PB9   Y        N         N
816   PB8   Y        N         N
817   PB5   Y        N         Y
818   PB4   Y        N         Y
819   PB3   Y        N         Y
820   PA15  Y        N         Y
821   PA10  Y        N         Y
822   PA9   Y        N         Y
823   PA8   Y        N         Y
824   PB15  Y        N         Y
825   PB14  Y        N         Y
826   PB13  Y        N         Y
827   PB12  Y        N         N


*/









NmraDcc  Dcc ;
DCC_MSG  Packet ;
PinPulser pinPulser;

uint16_t BaseTurnoutAddress;

/*
 * Rosscoe Train functions and variables
 */
// for address learning mode
#ifdef LEARNING
int learningMode = LOW;
#endif

// buffer to hold serial commands
String readString;


#ifdef SINGLE_PULSE
  uint16_t onMs;
  uint8_t  activeOutputState;
#else
  static uint16_t onMs[NUM_TURNOUTS] = {};
  static uint8_t activeOutputState[NUM_TURNOUTS] = {};
#endif

char* version;
uint8_t versionBuffer[3];


// this is for STM32F1xx versions but here for all as will affect control of DCC.
bool i2cControl = false;

#endif


