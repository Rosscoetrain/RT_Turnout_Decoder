/*
 * @file defines.h
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

#ifndef DEFINES_H
#define DEFINES_H


// this is the serial number for the board

#warning "Have you set the serial number"

#define SERIAL_NUMBER 47


// Un-Comment the line below to force CVs to be written to the Factory Default values
// defined in the FactoryDefaultCVs below on Start-Up
// THIS NEEDS to be un-commented and uploaded once to setup the eeprom
// after uploading comment out the line and upload again for normal operation
//#define FORCE_RESET_FACTORY_DEFAULT_CV

// Comment the line below to remove serial commands and display of messages on console
#define ENABLE_SERIAL

#ifdef ENABLE_SERIAL
// **** un-comment #define ENABLE_SERIAL above to use the following

#define MYSERIAL Serial

// You can print every DCC packet by un-commenting the line below (except for idle and service mode reset)
#define NOTIFY_DCC_MSG

// You can print every notifyDccAccTurnoutOutput call-back by un-commenting the line below
//#define NOTIFY_TURNOUT_MSG

// You can also print other Debug Messages uncommenting the line below
//#define DEBUG_MSG



#endif    // ENABLE_SERIAL

// Un-Comment the line below to include learning function
#define LEARNING
#ifdef LEARNING
#ifdef STM32F1xx
#define LEARNINGBUTTON PB13
#else
#define LEARNINGBUTTON A6
#endif
#endif



// Un-Comment the lines below to Enable DCC ACK for Service Mode Programming Read CV Capablilty

#ifdef STM32F1xx
#define ENABLE_DCC_ACK  PB12
#else
#define ENABLE_DCC_ACK  15  // This is A1 on the Iowa Scaled Engineering ARD-DCCSHIELD DCC Shield
#endif

// TODO need to define DCC_ACK pin for different processors.



// Un-Comment the line below to use a single output pulse time.
// The pulse time will be the same for all addresses
//
//#define SINGLE_PULSE

// Define the Arduino input Pin number for the DCC Signal

#ifdef STM32F1xx
#define DCC_PIN     PA8
#else
#define DCC_PIN     2
#endif


#define NUM_TURNOUTS 8                // Set Number of Turnouts (Pairs of Pins)
#define ACTIVE_OUTPUT_STATE HIGH      // Set the ACTIVE State of the output to Drive the Turnout motor electronics HIGH or LOW

#define DEFAULT_PULSE 10              // the default pulse ms/10

#define DEFAULT_RECHARGE 30           // the default CDU recharge time ms/10

#define DCC_DECODER_VERSION_NUM 10    // Set the Decoder Version - Used by JMRI to Identify the decoder

#define LEDCONTROL LED_BUILTIN


#define CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME 2  // CV for the Output Pulse ON ms
#define CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME 3  // CV for the delay in ms to allow a CDU to recharge
#define CV_ACCESSORY_DECODER_ACTIVE_STATE      4  // CV to define the ON Output State

#define CVS_PER_TURNOUT 2

#define CV_USER_GROUP_1 33                        // CV  33 -  81   maximum  48 CVs
#define CV_USER_GROUP_2 112                       // CV 112 - 128   maximum  16 CVs
#define CV_USER_GROUP_3 129                       // CV 129 - 256   maximum 127 CVs
#define CV_USER_GROUP_4 513                       // CV 513 - 895   maximum 382 CVs

#define CV_MAX_TURNOUTS (CV_USER_GROUP_1 + NUM_TURNOUTS * CVS_PER_TURNOUT)


#define CV_USER_GROUP CV_USER_GROUP_1



#define CV_ACCESSORY_DECODER_SERIAL_LSB 255       // lsb for board serial number
#define CV_ACCESSORY_DECODER_SERIAL_MSB 256       // msb for board serial number

#if defined (STM32F1xx_Blue_Pill) || defined (STM32F1xx_Stumpy)

#define I2C_ADDRESS 0x65

#define I2C_CONTROL PB4
#define I2C_SDA PB7
#define I2C_SCL PB6
#endif


#endif



