/*
 * @file functions.h
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



/*
 * a function to read an analogue pin and return a boolean value depending on reading.
 * works the same as doing a digital read on a digital pin
 * > 512 = TRUE, <= 512 = FALSE
 */
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "defines.h"

bool dr (int pin)
 {
  int val = analogRead(pin);
  return ( val > 512 ) ;
 }


#if defined (STM32F1xx_Blue_Pill) || defined (STM32F1xx_Stumpy)

/*
Disabling JTAG is required to avoid pin conflicts on Bluepill
*/


void disableJTAG()
 {
  // Disable JTAG and enable SWD by clearing the SWJ_CFG bits
  // Assuming the register is named AFIO_MAPR or AFIO_MAPR2
  AFIO->MAPR &= ~(AFIO_MAPR_SWJ_CFG);
  // or
  // AFIO->MAPR2 &= ~(AFIO_MAPR2_SWJ_CFG);
 }


/*
* Function to read I2C address from EEPROM
* address at 0
*
* TODO add validation
*
*/

uint8_t getI2CAddress()
 {
/*
  uint8_t eepromAddress;

  byte ee = EEPROM.read(0);

  if ((ee == 0) || (ee == 255))
   {
    EEPROM.write(0, I2C_ADDRESS);
    delay(50);
   }
  eepromAddress = EEPROM.read(0);
  return eepromAddress;
*/
  return 0;
 }

#endif

/*
 * setup the version number
 */


void setVersion() {
  const String versionString = VERSION;
  char versionArray[versionString.length() + 1];
  versionString.toCharArray(versionArray, versionString.length() + 1);
  version = strtok(versionArray, "."); // Split version on .
  versionBuffer[0] = atoi(version);  // Major first
  version = strtok(NULL, ".");
  versionBuffer[1] = atoi(version);  // Minor next
  version = strtok(NULL, ".");
  versionBuffer[2] = atoi(version);  // Patch last
}

/**
 * this is just a function to show via the onboard PCB led, the state of the decoder
 */

void showAcknowledge(int nb) {
    for (int i=0;i<nb;i++) {
    digitalWrite(LEDCONTROL, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);               // wait for a second
    digitalWrite(LEDCONTROL, LOW);    // turn the LED off by making the voltage LOW
    delay(100);               // wait for a second
  }
}

void showUserCVs()
 {
  for(uint8_t i = 0; i < NUM_TURNOUTS; i++)
   {
    MYSERIAL.print(F("CV"));
    MYSERIAL.print(33 + (i * 2));
    MYSERIAL.print(F(" = "));
    MYSERIAL.print(Dcc.getCV(33 + (i * 2)));
    MYSERIAL.print("    ");
    MYSERIAL.print(F("CV"));
    MYSERIAL.print(34 + (i * 2));
    MYSERIAL.print(F(" = "));
    MYSERIAL.println(Dcc.getCV(34 + (i * 2)));
  }
 }



void(* resetFunc) (void) = 0;                     // declare reset function at address 0


#ifdef ENABLE_SERIAL

#include "StringSplitter.h"


void serialCommandZ()
 {
    MYSERIAL.println(F("Resetting"));
    resetFunc();
 }

void serialCommandH()   // acutually <?>
 {
    MYSERIAL.println(F("Help Text"));
    MYSERIAL.println(F("Close a turnout: <C address>"));
    MYSERIAL.println(F("Throw a turnout: <T address>"));
    MYSERIAL.println(F("Set decoder base address: <A address>"));
#ifdef SINGLE_PULSE
    MYSERIAL.println(F("Set decoder output pulse time: <P  mS / 10>"));
    MYSERIAL.println(F("Set deocder active state: <S  0/1>"));
#else
    MYSERIAL.println(F("Set decoder output pulse time: <P output  mS / 10>"));
    MYSERIAL.println(F("Set decoder active state: <S output 0/1>"));
    MYSERIAL.println(F("Where output is 1 - 8 as on the decoder pcb"));
#endif
    MYSERIAL.println(F("Set decoder CDU recharge time: <R  mS / 10>"));
    MYSERIAL.println(F("Reset decoder to factory defaulst: <D>"));
    MYSERIAL.println(F("Show current CVs: <>"));
    MYSERIAL.println(F("Soft Reset: <Z>"));
 }

void serialCommandH2()  // actually <>
 {
  MYSERIAL.println(F("CVs are:"));
  MYSERIAL.print(F("CV"));
  MYSERIAL.print(CV_ACCESSORY_DECODER_ADDRESS_LSB);
  MYSERIAL.print(F(" = "));
  MYSERIAL.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB));
  MYSERIAL.print(F("CV"));
  MYSERIAL.print(CV_ACCESSORY_DECODER_ADDRESS_MSB);
  MYSERIAL.print(F(" = "));
  MYSERIAL.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB));
  MYSERIAL.print(F("CV"));
  MYSERIAL.print(CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME);
  MYSERIAL.print(F(" = "));
  MYSERIAL.println(Dcc.getCV(CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME));
  MYSERIAL.print(F("CV"));
  MYSERIAL.print(CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME);
  MYSERIAL.print(F(" = "));
  MYSERIAL.println(Dcc.getCV(CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME));

#ifdef SINGLE_PULSE
  MYSERIAL.print(F("CV"));
  MYSERIAL.print(CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME);
  MYSERIAL.print(F(" = "));
  MYSERIAL.println(Dcc.getCV(CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME));

  MYSERIAL.print(F("CV"));
  MYSERIAL.print(CV_ACCESSORY_DECODER_ACTIVE_STATE);
  MYSERIAL.print(F(" = "));
  MYSERIAL.println(Dcc.getCV(CV_ACCESSORY_DECODER_ACTIVE_STATE));
#else
  showUserCVs();
#endif
 }

void serialCommandC()
 {
  StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
  int itemCount = splitter->getItemCount();

  if ( itemCount == 2)
   {
    int addr = splitter->getItemAtIndex(1).toInt();
    notifyDccAccTurnoutOutput( addr, 0, 1 );
   }
  else
   {
    MYSERIAL.println(F("Invalid command: should be <C address>"));
   }
  delete splitter;
  splitter = NULL;
 }

void serialCommandT()
 {
  StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
  int itemCount = splitter->getItemCount();

  if ( itemCount == 2)
   {
    int addr = splitter->getItemAtIndex(1).toInt();
    notifyDccAccTurnoutOutput( addr, 1, 1 );
   }
  else
   {
    MYSERIAL.println(F("Invalid command: should be <T address>"));
   }
  delete splitter;
  splitter = NULL;
 }

void serialCommandY()          // test each address throw/colse on 500mS cycle.
 {
  for (int a = BaseTurnoutAddress; a < BaseTurnoutAddress + NUM_TURNOUTS; a++)
   {
    notifyDccAccTurnoutOutput( a, 1, 1 );
    delay(500);
    notifyDccAccTurnoutOutput( a, 0, 1 );
    delay(500);
   }
 }


void serialCommandA()
 {
  StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
  int itemCount = splitter->getItemCount();

  if ( itemCount == 2)
   {
    int addr = splitter->getItemAtIndex(1).toInt();

    byte L = (addr + 3) / 4;
    byte H = (addr + 3) / 1024;

#ifdef DEBUG_MSG
    MYSERIAL.print(F("Value = ")); MYSERIAL.println(addr);
    MYSERIAL.print(F(" H = ")); MYSERIAL.println(H);
    MYSERIAL.print(F(" L = ")); MYSERIAL.println(L);
#endif

    Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, H);
    Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, L);
   }
  else
   {
    MYSERIAL.println(F("Invalid command: should be <A address>"));
   }
  delete splitter;
  splitter = NULL;
 }

void serialCommandP()
 {
  StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
  int itemCount = splitter->getItemCount();

#ifdef SINGLE_PULSE
  if ( itemCount == 2)
   {
    int value = splitter->getItemAtIndex(1).toInt();
  #ifdef DEBUG_MSG
    MYSERIAL.print(F("Value = ")); MYSERIAL.println(value);
  #endif
    Dcc.setCV(CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME, value);
   }
  else
   {
    MYSERIAL.println(F("Invalid command: should be <P ms/10>"));
   }
  delete splitter;
  splitter = NULL;
#else
  if ( itemCount == 3)
   {
    int addr = splitter->getItemAtIndex(1).toInt();
    int value = splitter->getItemAtIndex(2).toInt();

  #ifdef DEBUG_MSG
    MYSERIAL.print(F("Adress = ")); MYSERIAL.println(addr);
    MYSERIAL.print(F("Value = ")); MYSERIAL.println(value);
  #endif
    if ( addr >= 1 && addr <= 8 )
     {
      Dcc.setCV(33 + (addr - 1) * 2, value);
     }
    else
     {
      MYSERIAL.println(F("Invalid output: should be 1 to 8"));
     }
   }
  else
   {
    MYSERIAL.println(F("Invalid command: should be <P output ms/10>"));
   }
  delete splitter;
  splitter = NULL;
#endif
 }


void serialCommandR()
 {
  StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
  int itemCount = splitter->getItemCount();

  if ( itemCount == 2)
   {
    int addr = splitter->getItemAtIndex(1).toInt();

#ifdef DEBUG_MSG
    MYSERIAL.print(F("Value = ")); MYSERIAL.println(addr);
#endif

    Dcc.setCV(CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME, addr);
   }
  else
   {
    MYSERIAL.println(F("Invalid command: should be <R ms/10>"));
   }
  delete splitter;
  splitter = NULL;
 }

void serialCommandS()
 {
  StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
  int itemCount = splitter->getItemCount();

#ifdef SINGLE_PULSE
  if (itemCount == 2)
   {
    int value = splitter->getItemAtIndex(1).toInt();

  #ifdef DEBUG_MSG
    MYSERIAL.print(F("Value = ")); MYSERIAL.println(value);
  #endif
    if ( value == 0 || value == 1 )
     {
      Dcc.setCV(CV_ACCESSORY_DECODER_ACTIVE_STATE, value);
     }
    else
     {
      MYSERIAL.println(F("Invalid value: state should be 0 or 1"));
     }
   }
  else
   {
    MYSERIAL.println(F("Invalid command: should be <S 0> or <S 1>"));
   }
  delete splitter;
  splitter = NULL;
#else
  if (itemCount == 3)
   {
    int addr = splitter->getItemAtIndex(1).toInt();
    int value = splitter->getItemAtIndex(2).toInt();

  #ifdef DEBUG_MSG
    MYSERIAL.print(F("Address = ")); MYSERIAL.println(addr);
    MYSERIAL.print(F("Value = ")); MYSERIAL.println(value);
  #endif

    Dcc.setCV(34 + (addr - 1) * 2, value);
   }
  else
   {
    MYSERIAL.println(F("Invalid command: should be <S address 0> or <S address 1>"));
   }
  delete splitter;
  splitter = NULL;
#endif
 }


void serialCommandD()
 {
  MYSERIAL.println(F("Reset factory default CVs"));
  notifyCVResetFactoryDefault();
 }


void doSerialCommand(String readString)
 {
  readString.trim();
  readString.toUpperCase();

  MYSERIAL.println(readString);                    // so you can see the captured string

if (readString == "<Z>")
   {
    serialCommandZ();
   }

  if (readString == "<?>")
   {
    serialCommandH();
   }
  else
   {
    if (readString.startsWith("<>"))
     {
      serialCommandH2();
     }
    else
     {
      if (readString.startsWith("<"))
       {
        // this is where commands are completed

        // command to close turnout <C address>
        if (readString.startsWith("<C"))
         {
          serialCommandC();
         }

        // show only the user CVs
        if (readString.startsWith("<U"))
         {
          showUserCVs();
         }

         // command to throw turnout <T address>
        if (readString.startsWith("<T"))
         {
          serialCommandT();
         }

        if (readString.startsWith("<A>"))
         {
          MYSERIAL.println(BaseTurnoutAddress);
          readString = "";
         }

         // command to set address <A address>
         // address will be adjusted to the correct base turnout address
         // eg if address is 2 this will be corrected to 1 as the address are groups of 8 with an offset of 4
         // ie 1..8, 5..12, ...

        if (readString.startsWith("<A"))
         {
          serialCommandA();
         }

/*
 * command to set output pulse time.
 * The value here is the number of milliseconds / 10
 * ie 100ms/10 = 10.
 *
 */
        if (readString.startsWith("<P"))
         {
          serialCommandP();
         }

/*
 * delay time for the CDU to recharge
 * The value here is the number of milliseconds / 10
 * ie 300ms/10 = 30
 */

        if (readString.startsWith("<R"))
         {
          serialCommandR();
         }

/*
 * the output state
 * 1 = high for output
 * 0 = low for output
 */

        if (readString.startsWith("<S"))
         {
          serialCommandS();
         }

        if (readString.startsWith("<X"))
         {
          MYSERIAL.println(F("Serial number is:"));
          MYSERIAL.println(Dcc.getCV(CV_ACCESSORY_DECODER_SERIAL_LSB) + (Dcc.getCV(CV_ACCESSORY_DECODER_SERIAL_MSB) * 256 ));
          MYSERIAL.println("");
         }


        if (readString == "<D>")
         {
//          MYSERIAL.println(F("Reset factory default CVs"));
//          notifyCVResetFactoryDefault();
          serialCommandD();
         }


        if (readString.startsWith("<Y>"))    // test each address throw/close 500mS cycle.
         {
          serialCommandY();
         }


        if (readString.startsWith("<W"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 3)
           {
            byte addr = splitter->getItemAtIndex(1).toInt();
            int value = splitter->getItemAtIndex(2).toInt();

            switch (addr) {
/*
              case CV_ACCESSORY_DECODER_ADDRESS_LSB:                  // CV1

                    byte L = (value + 3) / 4;
                    byte H = (value + 3) / 1024;

#ifdef DEBUG_MSG
                  MYSERIAL.print(F("Value = ")); MYSERIAL.println(value);
                  MYSERIAL.print(F(" H = ")); MYSERIAL.println(H);
                  MYSERIAL.print(F(" L = ")); MYSERIAL.println(L);
#endif

                  Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, H);
                  Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, L);
              break;
              case CV_ACCESSORY_DECODER_ADDRESS_MSB:                  // CV9
                  Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, value);
              break;
*/
              case 8:
                if (value == 8)
                 {
                 }
              break;
              case CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME:
                if ((value >= 0) && (value <= 255))
                 {
                  Dcc.setCV(CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME, value);
                 }
              break;
/*
              case CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME:
                if ((value >= 0) && (value <= 255))
                 {
                  Dcc.setCV(CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME, value);
                 }
              break;
              case CV_ACCESSORY_DECODER_ACTIVE_STATE:
                if ((value == 0) || (value == 1))
                 {
                  Dcc.setCV(CV_ACCESSORY_DECODER_ACTIVE_STATE, value);
                 }
                else
                 {
                  MYSERIAL.println(F("Value must be 0 (LOW) or 1 (HIGH)"));
                 }
              break;
*/
              default:
                 MYSERIAL.println(F("Invalid cv number: should be <W cv value> "));
              break;
             }
           }
          else
           {
            MYSERIAL.println(F("Invalid command: should be <W cv value>"));
           }
          delete splitter;
          splitter = NULL;
         }

       }
      else
       {
        MYSERIAL.println(F("ERROR: Unknown command"));
       }
     }
   }
 }

#endif // ENABLE_SERIAL


void initPinPulser(void)
{
  BaseTurnoutAddress = (((Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB) * 256) + Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB) - 1) * 4) + 1  ;

  uint16_t cduRechargeMs     = Dcc.getCV(CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME) * 10;

#ifdef SINGLE_PULSE
  onMs              = Dcc.getCV(CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME) * 10;
  activeOutputState = Dcc.getCV(CV_ACCESSORY_DECODER_ACTIVE_STATE);
#else
// read the CV's for each address
  for(uint8_t i = 0; i < NUM_TURNOUTS; i++)
  {
    onMs[i] = Dcc.getCV( 33 + ( i * 2 ) ) * Dcc.getCV(CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME);
    activeOutputState[i]  = Dcc.getCV( 34 + ( i * 2 ) );
  #ifdef DEBUG_MSG
    MYSERIAL.print(F(" i : "));
    MYSERIAL.print(i);
    MYSERIAL.print(F(" onMs : "));
    MYSERIAL.print(onMs[i]);
    MYSERIAL.print(F(" activeOutputState : "));
    MYSERIAL.println(activeOutputState[i]);
  #endif
  }
#endif

#ifdef ENABLE_SERIAL
//#ifdef DEBUG_MSG
  MYSERIAL.print(F("initPinPulser: DCC Turnout Base Address: "));
  MYSERIAL.print(BaseTurnoutAddress, DEC);
  MYSERIAL.print(F(" CDU Recharge: "));
  MYSERIAL.println(cduRechargeMs);
#endif
#ifdef SINGLE_PULSE
  MYSERIAL.print(F(" Active Pulse: ")); MYSERIAL.print(onMs); 
  MYSERIAL.print(F("ms Active Output State: ")); MYSERIAL.println(activeOutputState ? "HIGH" : "LOW" );
#endif

  // Step through all the Turnout Driver pins setting them to OUTPUT and NOT Active State
  for(uint8_t i = 0; i < (NUM_TURNOUTS * 2); i++)
  {
#ifdef SINGLE_PULSE
  	digitalWrite(outputs[i], !activeOutputState); // Set the Output Inactive before the direction so the
#else
    digitalWrite(outputs[i], !activeOutputState[i / 2]); // Set the Output Inactive before the direction so the
#endif
  	pinMode( outputs[i], OUTPUT );                // Pin doesn't momentarily pulse the wrong state
	}

  // Init the PinPulser with the new settings
#ifdef SINGLE_PULSE
  pinPulser.init(onMs, cduRechargeMs, activeOutputState);
#else
  pinPulser.init(onMs, cduRechargeMs, activeOutputState, outputs);

  pinPulser.printArrays();
#endif
}



#endif

