
/*
14CORE LCD 16x2 Display Driving Example
*/
#include <Wire.h> // Include Wire.h to control I2C
#include <LiquidCrystal_I2C.h> //Download & include the code library can be downloaded below
#include <SPI.h>
#include <SD.h>
File data;
 
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE); // Initialize LCD Display at address 0x27 
void setup()
{
  Serial.begin(9600);
  lcd.begin (16,2);
  Serial.print("SD Card init...");
  //test if wiring is correct
  if (!SD.begin(10)) {
    Serial.println("init failed..");
    while (1);
  }
  Serial.println("init ok");
}
 
void loop()
{
  String brightLevel, secondLevel;
  int value = analogRead(A0);
  int second = analogRead(A1);
  if(value<100){
    brightLevel = "Low";
  }else{
    brightLevel = "High";
  }
  if(second<100){
    secondLevel = "Low";
  }else{
    secondLevel = "High";
  }
  lcd.setBacklight(HIGH); //Set Back light turn On
  lcd.setCursor(0,0); // Move cursor to 0
  lcd.print(value); 
  lcd.setCursor(5,0);
  lcd.print(second);
  lcd.setCursor(0,2);
  lcd.print(brightLevel);
  lcd.setCursor(5,2);
  lcd.print(secondLevel);

  
  data = SD.open("data.txt", FILE_WRITE); //open file
  if (data) {
    data.print(value); //print the data to file
    data.print(",");
    data.print(second);
    data.println();

    data.close();
    Serial.println(value);
  } else {
    Serial.println("Cannot open file");
  }
  delay(1000);
  lcd.clear();
  
}
