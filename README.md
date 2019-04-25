# I2C_ExtEEPROM (Arduino)

Read / write objects (structure, array, float...) into and from an external I2C EEPROM

There is no checking here, the deal is to reduce code size.

_____________

Basic settings:
 - `ExtEEPROM.setClock(int clockFrequency)`, set the I2C clock (400 => 400kHz)
 - `ExtEEPROM.setID(byte addr_ID)`, set the ID address of the device (by default, this is 0x50)
 
Work on a single byte:
 - `ExtEEPROM.read(int addr)`, return the byte from the EEPROM
 - `ExtEEPROM.write(int addr, byte data)`, write the byte 'data' into the EEPROM
 - `ExtEEPROM.check(int addr, byte data)`, same than 'write', but only if the byte into the EEPROM is different
 
Work on any object (structure, array, float...):
 - `ExtEEPROM.get(int addr, object)`, read from the EEPROM to fill the object
 - `ExtEEPROM.put(int addr, const object)`, write into EEPROM the content of the object
 - `ExtEEPROM.update(int addr, const object)`, same than 'put', but only if the byte into the EEPROM is different
