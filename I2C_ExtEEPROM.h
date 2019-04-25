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



#ifndef I2C_ExtEEPROM_h
#define I2C_ExtEEPROM_h





#include <Arduino.h>
#include <Wire.h>




class I2C_ExtEEPROMClass
{
  public:
    I2C_ExtEEPROMClass();
    
    void      setClock(unsigned int clockFrequency);
    void      setID(byte addr_ID);

    // to work on 1 byte
    byte      read(unsigned int addr);
    void      write(unsigned int addr, byte data);
    void      check(unsigned int addr, byte data);

    // to 'get', 'put' and 'update' objects to and from EEPROM
    template <typename T>       T     &get(unsigned int addr, T &value)
    {
      byte* p = (byte*)(void*)&value;
      byte  i = sizeof(value);
      while(i--)  *p++ = read(addr++);
    };
    template <typename T> const T     &put(unsigned int addr, const T &value)
    {
      const byte* p = (const byte*)(const void*)&value;
      byte        i = sizeof(value);
      while(i--)  write(addr++, *p++);
    };
    template <typename T> const T     &update(unsigned int addr, const T &value)
    {
      const byte* p = (const byte*)(const void*)&value;
      byte        i = sizeof(value);
      while(i--)  check(addr++, *p++);
    };

  private:
    byte      _addr_ID;
    
    void      _set(unsigned int addr);
};

extern I2C_ExtEEPROMClass ExtEEPROM;



#endif  // I2C_ExtEEPROM_h
