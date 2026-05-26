/*
 *  © 2023, Peter Cole. All rights reserved.
 *  © 2026, Ross Scanlon.
 * 
 *  Modified to add into RT_Turnout_Decoder
 *  @file display_functions.h
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

#ifndef DISPLAY_FUNCTIONS_H
#define DISPLAY_FUNCTIONS_H

#include <Arduino.h>
#include "globals.h"

#ifndef RT_TURNOUT_DECODER
extern uint8_t displayEvent;
extern uint8_t displayEventFlag;
#else
uint8_t displayEvent;
uint8_t displayEventFlag;
#endif

#ifndef RT_TURNOUT_DECODER
void setVersion() {};           use original in RT_Turnout_Decoder functions.h
#endif

void displayPins() {};
void displayVpinMap() {};
void processDisplayOutput() {};
void startupDisplay() {};

#endif
