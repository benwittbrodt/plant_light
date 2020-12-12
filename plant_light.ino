
#include <Wire.h> // Include Wire.h to control I2C display
#include <LiquidCrystal_I2C.h> //Downloaded library for I2C display
#include <SPI.h> //Library for SD card
#include <SD.h> //Library for SD card

File data;
 
#define SD_CARD_PIN 10 //Change for the PIN that the CS output from SD card module is connected to

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE); // Initialize LCD Display at address 0x27 

void setup()
{
  //Initialize serial port and lcd 
  Serial.begin(9600);
  lcd.begin (16,2);

  Serial.print("SD Card init...");
  //test if wiring is correct
  if (!SD.begin(SD_CARD_PIN)) {
    Serial.println("init failed..");
    while (1);
  }
  Serial.println("init ok");
}
 
void loop()
{
  String firstLevel, secondLevel; //giving a "class" of brightness for LCD output

  int first = analogRead(A0);
  int second = analogRead(A1);

  //100 is a arbitrary value used for testing the output and photoresistor
  if(first<100){
    firstLevel = "Low";
  }else{
    firstLevel = "High";
  }
  if(second<100){
    secondLevel = "Low";
  }else{
    secondLevel = "High";
  }

  lcd.setBacklight(HIGH); //Set Back light turn On
  
  lcd.setCursor(0,0); // Move cursor to 0
    lcd.print(first); 
  lcd.setCursor(5,0);
    lcd.print(second);
  lcd.setCursor(0,1);
    lcd.print(firstLevel);
  lcd.setCursor(5,1);
    lcd.print(secondLevel);

  
  data = SD.open("data.txt", FILE_WRITE); //open file
  if (data) {
    data.print(first);  //print the data to file
    data.print(",");    //Manually creating csv logic - to be fixed later 
    data.print(second); //second sensor reading
    data.println();     //create new line for data

    data.close();

    //just outputting one value to the serial monitor to ensure data is sending correctly
    Serial.println(first);
  
  } else {
    //if you cannot open the data file it will give an error in serial monitor
    Serial.println("Cannot open file");
  }

  delay(1000);
  
  lcd.clear(); //Clearing display
  
}
