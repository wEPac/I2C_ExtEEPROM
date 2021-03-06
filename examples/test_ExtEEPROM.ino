/*
   a test for I2C_ExtEEPROM
        copy and paste the 2 files ""I2C_ExtEEPROM.h" and "I2C_ExtEEPROM.h",
        into the same folder than this file "I2C_ExtEEPROM.ino"

   perform:
        - a scan for I2C bus to find devices
        - a speed test to set the clock at max speed
        - a test to write & read different object (structure, array, float...)

   adapt at your needs:
        - IDaddr, address of the device
        - startAddr, where to start to read write in the EEPROM

   I2C / TWI pins:
        Board            SDA        SCL
        ------------------------------------
        Uno, Ethernet    A4         A5
        Mega2560         20         21
        Leonardo         2          3
        Due              20, SDA1   21, SCL1

*/



#include <Wire.h>
#include "I2C_ExtEEPROM.h"

byte  IDaddr      = 0x57;   // address of the device
int   startAddr   = 256;

struct Test                 // Create a test structure
{
  int32_t x;
  float   y;
  uint8_t z;
}     sValues     = { -1847483647, 1.23483748, 0x08 };



void setup()
{
  Serial.begin(115200);     // Initialise the Serial port
  while (!Serial) {}        // Leonardo: wait for serial port to connect



  // default address of the device
  ExtEEPROM.setID(IDaddr);  // EEPROM ID address (default 0x50)

  // clockFrequency (option and here, it is set by the 'scanSpeed()' test):
  //      10    =>    10kHz       low
  //      100   =>    100kHz      [default]
  //      400   =>    400kHz      fast
  //      1000  =>    1000kHz     fast+
  //      3400  =>    3400kHz     high speed (unavaible here, uint8_t)
  //ExtEEPROM.setClock(400);



  scanI2C();
  scanSpeed();
  testEEPROM_R_W();
}

void loop() {}



void testEEPROM_R_W()
{
  Serial.println(); Serial.println(); Serial.println();
  Serial.println("¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤");
  Serial.println("Begin Read/Write test ...");
  ExtEEPROM.put(startAddr, sValues);    // Write the test structure to EEPROM

  int32_t valueX;
  float   valueY;
  uint8_t valueZ;

  Serial.println("==============================");
  Serial.println("Read the EEPROM values individually");
  ExtEEPROM.get(startAddr + (uint32_t)&sValues.y - (uint32_t)&sValues, valueY);
  ExtEEPROM.get(startAddr + (uint32_t)&sValues.z - (uint32_t)&sValues, valueZ);
  ExtEEPROM.get(startAddr + (uint32_t)&sValues.x - (uint32_t)&sValues, valueX);
  Serial.println(valueX);
  Serial.println(valueY, 6);
  Serial.println(valueZ);

  Serial.println("==============================");
  Serial.println("Change the float value");
  ExtEEPROM.put(startAddr + (uint32_t)&sValues.y - (uint32_t)&sValues, (float)3.35343);
  ExtEEPROM.get(startAddr + (uint32_t)&sValues.y - (uint32_t)&sValues, valueY);
  ExtEEPROM.get(startAddr + (uint32_t)&sValues.z - (uint32_t)&sValues, valueZ);
  ExtEEPROM.get(startAddr + (uint32_t)&sValues.x - (uint32_t)&sValues, valueX);
  Serial.println(valueX);
  Serial.println(valueY, 6);
  Serial.println(valueZ);

  Serial.println("==============================");
  Serial.println("Read the full structure from EEPROM");
  ExtEEPROM.get(startAddr, sValues);
  Serial.println(sValues.x);
  Serial.println(sValues.y, 6);
  Serial.println(sValues.z);

  Serial.println("==============================");
  Serial.println("Read a word = 14598");
  word w = 14598;
  ExtEEPROM.put(startAddr, w);
  word wBuff;
  ExtEEPROM.get(startAddr, wBuff);
  Serial.println(wBuff);

  Serial.println("==============================");
  Serial.println("Read an array = {1,2,3,4,5,6}");
  byte a[] = {1, 2, 3, 4, 5, 6};
  ExtEEPROM.put(startAddr, a);
  byte aBuff[sizeof(a)];
  ExtEEPROM.get(startAddr, aBuff);
  for (byte n = 0; n < sizeof(a); n++)
  {
    Serial.println(aBuff[n]);
  }

  Serial.println("¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤");
  Serial.println("End Read/Write test");
}



void scanI2C()
{
  Serial.println(); Serial.println(); Serial.println();
  Serial.println("¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤");
  Serial.println("I2C scanner. Scanning ...");
  Serial.println("==============================");

  byte count = 0;

  Wire.begin();
  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0)
    {
      Serial.print(" Found address: ");
      Serial.print(i, DEC);
      Serial.print(" (0x"); Serial.print(i, HEX); Serial.println(")");
      count++;
      delay(1);  // maybe unneeded?
    }
  }
  Serial.println(" Done.");
  Serial.println();
  Serial.print("Found "); Serial.print(count, DEC); Serial.println(" device(s).");
  Serial.println("¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤");
  Serial.println("End scanning");
}

void scanSpeed()
{
  Serial.println(); Serial.println(); Serial.println();
  Serial.println("¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤");
  Serial.print("I2C Speed test for '0x"); Serial.print(IDaddr, HEX); Serial.println("' ...");
  Serial.println("==============================");

  unsigned long speeds[]  = {3400, 1000, 400, 100, 10};
  byte          i         = sizeof(speeds) / 4;
  byte          speed_max = i;
  while (i--)
  {
    Serial.print(" clock at ");
    Serial.print((speeds[i] < 100) ? " " : "");
    Serial.print((speeds[i] < 1000) ? " " : "");
    Serial.print(speeds[i]); Serial.print("kHz : I2C ");
    byte success = false;

    Wire.setClock(speeds[i] * 1000);
    Wire.beginTransmission(IDaddr);
    if (Wire.endTransmission() == 0)
    {
      byte tag = 0xCE;
      ExtEEPROM.put(startAddr, tag);
      if (tag == ExtEEPROM.read(startAddr))
      {
        success   = true;
        speed_max = i;
      }
    }

    if (success) Serial.println("succeed");
    else         Serial.println("failed");
  }
  Serial.println();
  Serial.print("Set the clock at "); Serial.print(speeds[speed_max]); Serial.println("kHz");
  Serial.println("¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤");
  Serial.println("End Speed test");
  Wire.setClock(speeds[speed_max] * 1000UL);
}



