/*  ============================================================================
 *  Written by Eric Paquot, 03/2019
 *  https://github.com/wEPac
 *  ============================================================================
 *  
 *  Inspired by tiny code parts got here and there
 * 
 *  Read/Write multiple bytes to and from an I2C EEPROM.
 *      Valid device ID address range is 0x00~0xFF.
 *      Valid address range is 0x0000~0xFFFF, no checking.
 * 
 *  Features:
 *      - Not the fastest way but a very tiny code size.
 *      - Able to manage objects
 *      - Able to exceed the limit of 31 bytes of the Wire library
 *      - Able to update instead of writing
 *      
 *  ============================================================================
 *  
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 * 
 *  ============================================================================*/



#include "I2C_ExtEEPROM.h"
#include <Wire.h>



byte _addr_ID = 0x50;           // default address of the device



// Constructor
I2C_ExtEEPROMClass ExtEEPROM;

I2C_ExtEEPROMClass::I2C_ExtEEPROMClass()
{
  Wire.begin();                 // it can be started several times with other files, so dont worry
}



// clockFrequency:
//      10    =>    10kHz       low
//      100   =>    100kHz      [default]
//      400   =>    400kHz      fast
//      1000  =>    1000kHz     fast+
//      3400  =>    3400kHz     high speed (unavaible here, uint8_t)
void I2C_ExtEEPROMClass::setClock(unsigned int clockFrequency)
{
  Wire.setClock(clockFrequency * 1000);
  delay(10);
}



// addr_ID, address of the device
void I2C_ExtEEPROMClass::setID(byte addr_ID)
{
  _addr_ID = addr_ID;
}



void I2C_ExtEEPROMClass::_set(unsigned int addr)
{
  Wire.beginTransmission(_addr_ID);
  Wire.write((int)(addr >> 8));   // MSB
  Wire.write((int)(addr & 0xFF)); // LSB
}

byte I2C_ExtEEPROMClass::read(unsigned int addr) 
{
  byte data = 0xFF;

  _set(addr);
  Wire.endTransmission();
 
  Wire.requestFrom(int(_addr_ID), 1);
  if (Wire.available())  data = Wire.read();
  
  return data;
}

void I2C_ExtEEPROMClass::write(unsigned int addr, byte data) 
{ 
  _set(addr);
  Wire.write(data);
  Wire.endTransmission();
  
  delay(5);
}

void I2C_ExtEEPROMClass::check(unsigned int addr, byte data)
{
  if (data != read(addr)) write(addr, data);
}



