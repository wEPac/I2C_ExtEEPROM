# I2C_ExtEEPROM

Read / write objects (structure, array, float...) into and from an external I2C EEPROM

There is no checking here, the deal is to reduce code size.

=============================================

Basic settings:
 - `ExtEEPROM.setClock(uint16_t clockFrequency)`, set the I2C clock (clockFrequency == 400 => 400kHz)
 - `ExtEEPROM.setID(byte addr_ID)`, set the ID address of the device (by default, this is 0x50)
 
Work on a single byte:
 - `ExtEEPROM.read(unsigned int addr)`, return the byte from the EEPROM
 - `ExtEEPROM.write(unsigned int addr, byte data)`, write the byte 'data' into the EEPROM
 - `ExtEEPROM.check(unsigned int addr, byte data)`, same than 'write', but only if the byte into the EEPROM is different than data
 
Work on any objects (structure, array, float...):
 - `ExtEEPROM.get(int addr, object)`, read from the EEPROM to fill the object
 - `ExtEEPROM.put(int addr, const object)`, write into EEPROM the content of the object
 - `ExtEEPROM.update(int addr, const object)`, same than 'put', but only if the byte into the EEPROM is different
