// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>

File datafile;

RTC_DS1307 rtc;

//Defining the desplay we are using (here it is at 0x27 I2C address and is a 16 character, 2 row display)
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Each sensor has a colored wire - they are declared by the color until a final layout is determined, then I might rename per location
const byte BLUE_SENSOR = A4;
const byte GREEN_SENSOR = A5;

const int SD_CARD_PIN = 10;

//Interval for data pings - default is 30 seconds
const int dataInterval = 30000;

void setup()
{
  Serial.begin(9600);
  //Likely will remove later but for troubleshooting we are initiating the LCD screen, activating the backlight, and then setting it to HIGH
  lcd.init();
  lcd.backlight();
  lcd.setBacklight(HIGH);

  //Checks for the RTC module - if none are available it will abort
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (!rtc.isrunning())
  {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  Serial.print("SD Card init...");
  //test if wiring is correct
  if (!SD.begin(SD_CARD_PIN))
  {
    Serial.println("init failed..");
    while (1)
      ;
  }
  Serial.println("init ok");
  datafile = SD.open("20210202.txt", FILE_WRITE);
  datafile.print("Time");
  datafile.print(",");
  datafile.print("blue value");
  datafile.print(",");
  datafile.print("green value");
  datafile.println();
  datafile.close();
  if (datafile)
  {
    Serial.println("headers written");
  }
}

void loop()
{
  //Handling all of the time information//////////////
  String the_time;
  DateTime time = rtc.now();

  //Formatting the time output as timestamp - then printing at the top of the LCD for testing
  Serial.println(time.timestamp(DateTime::TIMESTAMP_FULL));
  Serial.println();
  the_time = time.timestamp(DateTime::TIMESTAMP_FULL);
  lcd.home();
  lcd.print(time.timestamp(DateTime::TIMESTAMP_TIME));

  ///////////////////////////////////////////////////////

  // Reading the info from analog sensors

  int blueValue = analogRead(BLUE_SENSOR);
  int greenValue = analogRead(GREEN_SENSOR);

  lcd.setCursor(0, 1);
  lcd.print(blueValue);

  lcd.setCursor(5, 1);
  lcd.print(greenValue);

  //////////////////////////////////////////////////////

  //Handling getting the data in to SD card

  datafile = SD.open("20210202.txt", FILE_WRITE);

  if (datafile)
  {
    datafile.print(the_time);
    datafile.print(",");
    datafile.print(String(blueValue));
    datafile.print(",");
    datafile.print(String(greenValue));
    datafile.println();

    datafile.close();
  }
  else
  {
    //if you cannot open the data file it will give an error in serial monitor
    Serial.println("Cannot open file");
  }
  /////////////////////////////////////////////////////

  //data delay second delay and clear the LCD so numbers don't overlap - if we don't clear the LCD the "old" data will stay when we enter the loop again
  delay(dataInterval);
  lcd.clear();
}
