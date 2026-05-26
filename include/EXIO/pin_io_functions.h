/*
 *  © 2023, Peter Cole. All rights reserved.
 *  © 2026, Ross Scanlon.
 * 
 *  Modified to add into RT_Turnout_Decoder
 *  @file pin_io_functions.h
 *  
 *  
 *  This file is part of EX-IOExpander.
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with CommandStation.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PIN_IO_FUNCTIONS_H
#define PIN_IO_FUNCTIONS_H

#include <Arduino.h>
#include "globals.h"


/*
 * these are placeholders at the moment. 23/05/2026
 */


void setupPinDetails() {};
void initialisePins() {};
bool enableDigitalInput(uint8_t pin, bool pullup) {return true;};
bool writeDigitalOutput(uint8_t pin, bool state) {return true;};
bool enableAnalogue(uint8_t pin) {return false;};
bool writeAnalogue(uint8_t pin, uint16_t value, uint8_t profile=0, uint16_t duration=0) {return false;};
void processInputs() {};
bool processOutputTest(bool testState) {return true;};

#endif
