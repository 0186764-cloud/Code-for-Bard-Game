#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Wire.h>;
//Manages Library by adding certain aspects
const int led = 12;
int DiceRoll;
int outcome;
//Declares integers

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Starts Serial Monitor
  lcd.init();
  lcd.backlight();
  display.begin();

  display.setContrast(50);

  display.clearDisplay(); //Resets the Nokia module before use
  //display.fillCircle(32, 24, 5, BLACK);
  //display.fillCircle(52, 24, 5, BLACK);
  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:
  Rolldie();
  Serial.println(DiceRoll);
}

void Rolldie(){
  int DiceRoll = random(1, 7);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.print("Value: ");
  display.print(DiceRoll);
  display.display();
  int outcome = DiceRoll;
  if (outcome < 4){
      lcd.setCursor(6, 0);
      lcd.print("It's");
      lcd.setCursor(6,1);
      lcd.print("Calm");
    }
    else if (outcome > 5){
      lcd.setCursor(6, 0);
      lcd.print("It's");
      lcd.setCursor(3,1);
      lcd.print("A CALAMITY");
    }
    else{
      lcd.setCursor(6, 0);
      lcd.print("It's");
      lcd.setCursor(5,1);
      lcd.print("A Storm");
    }
  delay(2500);
  lcd.clear();
  display.clearDisplay();
}
