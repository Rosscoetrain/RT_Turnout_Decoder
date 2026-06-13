/*
 * @file functions_dcc.h
 *  DCC functions
*/

#ifndef FUNTCIONS_DCC_H
#define FUNCTIONS_DCC_H

#include <EEPROM.h>

// This function is called whenever a normal DCC Turnout Packet is received
void notifyDccAccTurnoutOutput( uint16_t Addr, uint8_t Direction, uint8_t OutputPower )
 {
#ifdef  NOTIFY_TURNOUT_MSG
  MYSERIAL.print("notifyDccAccTurnoutOutput: Turnout: ") ;
  MYSERIAL.print(Addr,DEC) ;
  MYSERIAL.print(" Direction: ");
  MYSERIAL.print(Direction ? "Thrown" : "Closed") ;
  MYSERIAL.print(" Output: ");
  MYSERIAL.print(OutputPower ? "On" : "Off") ;
#endif

// check to see if in learning mode and update address

#ifdef LEARNING
  if (learningMode == HIGH) {

//    int H = (Addr - 1) / 64;
//    int L = Addr - (H * 64);
    byte L = (Addr + 3) / 4;
    byte H = (Addr + 3) / 1024;

  #ifdef DEBUG_MSG
    MYSERIAL.println("");
    MYSERIAL.print(F("Value = ")); MYSERIAL.println(Addr,DEC);
    MYSERIAL.print(F(" H = ")); MYSERIAL.println(H,DEC);
    MYSERIAL.print(F(" L = ")); MYSERIAL.println(L,DEC);
  #endif
    Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, H);
    Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, L);
   }
  else
#endif

   {
    if(( Addr >= BaseTurnoutAddress ) && ( Addr < (BaseTurnoutAddress + NUM_TURNOUTS )) && OutputPower )
     {
      uint16_t pinIndex = ( (Addr - BaseTurnoutAddress) << 1 ) + Direction ;
      pinPulser.addPin(outputs[pinIndex]);
#ifdef  NOTIFY_TURNOUT_MSG
      MYSERIAL.print(" Pin Index: ");
      MYSERIAL.print(pinIndex,DEC);
      MYSERIAL.print(" Pin: ");
      MYSERIAL.print(outputs[pinIndex],DEC);
#endif
     }
   }
#ifdef  NOTIFY_TURNOUT_MSG
  MYSERIAL.println();
#endif
 }

#if defined(STM32F1xx_Blue_Pill) || defined(STM32F1xx_Stumpy)
// Callback for when the command station requests a CV read
uint8_t notifyCVRead(uint16_t CV) {
  return EEPROM.read(CV);
}
#endif


void notifyCVChange(uint16_t CV, uint8_t Value)
 {

#if defined (STM32F1xx_Blue_Pill) || defined (STM32F1xx_Stumpy)
  if (EEPROM.read(CV != Value))
   {
    EEPROM.write(CV, Value);
    Serial.print("CV Written: ");
    Serial.print(CV);
    Serial.print(" = ");
    Serial.println(Value);
    EEPROM.commit();
   }
#endif

#ifdef DEBUG_MSG
  MYSERIAL.print("notifyCVChange: CV: ") ;
  MYSERIAL.print(CV,DEC) ;
  MYSERIAL.print(" Value: ") ;
  MYSERIAL.println(Value, DEC) ;
#endif

  Value = Value;  // Silence Compiler Warnings...

  if((CV == CV_ACCESSORY_DECODER_ADDRESS_MSB) || (CV == CV_ACCESSORY_DECODER_ADDRESS_LSB) ||
		 (CV == CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME) || (CV == CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME) || (CV == CV_ACCESSORY_DECODER_ACTIVE_STATE))
		initPinPulser();	// Some CV we care about changed so re-init the PinPulser with the new CV settings
 }

void notifyCVResetFactoryDefault()
{
  // Make FactoryDefaultCVIndex non-zero and equal to num CV's to be reset
  // to flag to the loop() function that a reset to Factory Defaults needs to be done
  FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs)/sizeof(CVPair);
};

// This function is called by the NmraDcc library when a DCC ACK needs to be sent
// Calling this function should cause an increased 60ma current drain on the power supply for 6ms to ACK a CV Read
#ifdef  ENABLE_DCC_ACK
void notifyCVAck(void)
 {
  #ifdef DEBUG_MSG
  MYSERIAL.println("notifyCVAck") ;
  #endif
  // Configure the DCC CV Programing ACK pin for an output  // TODO move this to setup
//  pinMode( ENABLE_DCC_ACK, OUTPUT );
  // Generate the DCC ACK 60mA pulse
  digitalWrite( ENABLE_DCC_ACK, HIGH );
  delay( 10 );  // The DCC Spec says 6ms but 10 makes sure... ;)
  digitalWrite( ENABLE_DCC_ACK, LOW );
 }
#endif



/*

// pom using loco address space

void notifyDccMsg(DCC_MSG *Msg)
 {
// ignore idle messages and service mode reset

  if (Msg->Data[0] == 0xFF || Msg->Data[0] == 0x7F || Msg->Data[0] == 0x00)
   {
    return;
   }

  #ifdef NOTIFY_DCC_MSG
    MYSERIAL.print("notifyDccMsg: ") ;
    for(uint8_t i = 0; i < Msg->Size; i++)
     {
      MYSERIAL.print(Msg->Data[i], HEX);
      MYSERIAL.write(' ');
     }
    MYSERIAL.println();
  #endif

// 1. Service Mode CV Write (Pattern 0x70)
  if ((Msg->Data[0] & 0xF0) == 0x70)
   {
    uint16_t cv = (((Msg->Data[0] & 0x03) << 8) | Msg->Data[1]) + 1;
    uint8_t val = Msg->Data[2];
    Dcc.setCV(cv, val);
    BaseTurnoutAddress = Dcc.getAddr();
   }

// 2. POM (Programming on Main) (Pattern 0xE0)
  if (Msg->Size >= 4)
   {
    if (Msg->Data[0] == BaseTurnoutAddress)
     {
      if ((Msg->Data[1] & 0xF0) == 0xE0)
       {
        uint16_t cv = (((Msg->Data[1] & 0x03) << 8) | Msg->Data[2]) + 1;
        uint8_t val = Msg->Data[3];
#ifdef NOTIFY_DCC_MSG
        MYSERIAL.print("CV : ");
        MYSERIAL.print(cv);
        MYSERIAL.print(" value : ");
        MYSERIAL.println(val);
#endif
        if ((cv == 8) && (val == 8))
         {
          serialCommandD();
          return;
         }
        Dcc.setCV(cv, val);
        BaseTurnoutAddress = Dcc.getAddr();
       }
     }
   }
 }
*/


void notifyDccMsg(DCC_MSG *Msg)
 {
// ignore idle messages and service mode reset

  if (Msg->Data[0] == 0xFF || Msg->Data[0] == 0x7F || Msg->Data[0] == 0x00)
   {
    return;
   }

  #ifdef NOTIFY_DCC_MSG
    MYSERIAL.print("notifyDccMsg: ") ;
    for(uint8_t i = 0; i < Msg->Size; i++)
     {
      MYSERIAL.print(Msg->Data[i], HEX);
      MYSERIAL.write(' ');
     }
    MYSERIAL.println();
  #endif

/*
// 1. Service Mode CV Write (Pattern 0x70)
  if ((Msg->Data[0] & 0xF0) == 0x70)
   {
    uint16_t cv = (((Msg->Data[0] & 0x03) << 8) | Msg->Data[1]) + 1;
    uint8_t val = Msg->Data[2];
    Dcc.setCV(cv, val);
    BaseTurnoutAddress = Dcc.getAddr();
   }

// 2. POM (Programming on Main) for Accessory Decoders (NMRA 2.4.3.1)
  if (Msg->Size >= 5)
   {
    // Validate this is an accessory decoder packet prefix (10xxxxxx)
    if ((Msg->Data[0] & 0xC0) == 0x80)
     {
      // Check for the programming flag in byte 1 (bit 3 must be 1)
      if ((Msg->Data[1] & 0x08) == 0x08)
       {
        // Reconstruct the 11-bit linear address scalar with correct shifts
        uint16_t msgAddr = ((~Msg->Data[1] & 0x70) << 4) |
                           ((Msg->Data[0] & 0x3F) << 2) |
                           ((Msg->Data[1] >> 1) & 0x03);


#ifdef NOTIFY_DCC_MSG
          MYSERIAL.print("msgAddr : ");
          MYSERIAL.println(msgAddr);
#endif


        if (msgAddr == BaseTurnoutAddress)
         {
          // For accessory POM, the long form config byte is in Msg->Data[2]
          // Pattern 0xEC indicates a Direct Long Form CV Write (GG=11)
          if ((Msg->Data[2] & 0xF0) == 0xE0)
           {
            // Extract 10-bit CV address from instruction byte and address byte
            uint16_t cv = (((Msg->Data[2] & 0x03) << 8) | Msg->Data[3]) + 1;
            uint8_t val = Msg->Data[4];

#ifdef NOTIFY_DCC_MSG
            MYSERIAL.print("POM Match! 11-bit Linear Address: ");
            MYSERIAL.print(msgAddr);
            MYSERIAL.print(" CV : ");
            MYSERIAL.print(cv);
            MYSERIAL.print(" value : ");
            MYSERIAL.println(val);
#endif

            if ((cv == 8) && (val == 8))
             {
              serialCommandD();
              return;
             }
            Dcc.setCV(cv, val);
            BaseTurnoutAddress = Dcc.getAddr();
           }
         }
       }
     }
   }
*/
 }


uint8_t notifyCVWrite (uint16_t CV, uint8_t Value)
 {

#ifdef DEBUG_MSG
  MYSERIAL.print("notifyCVWrite cv : ");
  MYSERIAL.print(CV);
  MYSERIAL.print(" value : ");
  MYSERIAL.println(Value);
#endif


  if (CV <= CV_MAX_TURNOUTS)
   {
    if ((CV == 8) && (Value == 8))  // factory reset return before writing to CV
     {
      serialCommandD();
      return Value;
     }
    if (CV == 8)                    // ignore any attempts to write to CV8
     {
      return Value;
     }
    if ((CV == 1) || (CV == 9))     // ignore writes to CV1 and CV9 causes problems with actual address
     {
      return Value;
//      EEPROM.update(CV, Value);
//      BaseTurnoutAddress = Dcc.getAddr();
     }
    if (((CV >= CV_USER_GROUP_1) && (CV <= CV_MAX_TURNOUTS)) ||
         CV == CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME ||
         CV == CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME ||
         CV == CV_ACCESSORY_DECODER_ACTIVE_STATE)
     {
      if (((CV - CV_USER_GROUP_1) % 2) == 1)
       {
        if (Value > 1)              // ignore values for CV34 + output x 2 that are greater than 1 only accepts 0 or 1
         {
          return Value;
         }
       }
      EEPROM.update(CV, Value);
      initPinPulser();
     }

   }

  return Value;
 }



#endif
