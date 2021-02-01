// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27,16,2);

//Handling pin numbers for each sensor
const byte BLUE_SENSOR = A4;
const byte GREEN_SENSOR = A5; 



void setup () {
  Serial.begin(57600);
  lcd.init();
  lcd.backlight();
  lcd.setBacklight(HIGH);

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  }

}

void loop () {
    //Handling all of the time information 
    
    DateTime time = rtc.now();
    
    Serial.println(time.timestamp(DateTime::TIMESTAMP_FULL));
    Serial.println();
  
    lcd.home();
      lcd.print(time.timestamp(DateTime::TIMESTAMP_TIME));
      
    ///////////////////////////////////////////////////////
    int blueValue = analogRead(BLUE_SENSOR);
    int greenValue = analogRead(GREEN_SENSOR);
    lcd.setCursor(0,1);
      lcd.print(blueValue);
    lcd.setCursor(5,1`````````````````````````````````````````````````````````````````````````````````````````````````);
      lcd.print(greenValue);
    // Reading the info from analog sensors
    
    
    //////////////////////////////////////////////////////
    delay(1000);
    lcd.clear();


    
}
