#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Wire.h>;

const int led = 13;
int outcome;
int DiceRoll;
int InitialPots = 10;

int PIB = -1;
int PRB = 0;
int POS = -1;

int step = 1;

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  lcd.init();
  lcd.backlight();
  display.begin();

  display.setContrast(50);
  display.clearDisplay();
  Serial.println("You have 10 pots.");
  Serial.println("How many would you like to put at bay?");
}

void Outcome(int DiceRoll) {
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.print("You Rolled: ");
  display.print(DiceRoll);
  display.display();
  int outcome = DiceRoll;
  if (outcome < 4) {
    lcd.setCursor(6, 0);
    lcd.print("It's");
    lcd.setCursor(6, 1);
    lcd.print("Calm");
  } else if (outcome > 5) {
    lcd.setCursor(6, 0);
    lcd.print("It's");
    lcd.setCursor(3, 1);
    lcd.print("A CALAMITY");
    digitalWrite(led, HIGH);
  } else {
    lcd.setCursor(6, 0);
    lcd.print("It's");
    lcd.setCursor(5, 1);
    lcd.print("A Storm");
  }
}

void RollDice(int DiceRoll){
  if (DiceRoll > 0){
    display.fillCircle(32, 12, 4, BLACK);
    display.display();
    delay(1000);

    if (DiceRoll > 1){
      display.fillCircle(52, 12, 4, BLACK);
      display.display();
      delay(1000);

      if (DiceRoll > 2){
        display.fillCircle(32, 24, 4, BLACK);
        display.display();
        delay(1000);

        if (DiceRoll > 3){
          display.fillCircle(52, 24, 4, BLACK);
          display.display(); 
          delay(1000);

          if (DiceRoll > 4){
            display.fillCircle(32, 36, 4, BLACK);
            display.display();
            delay(1000);

            if (DiceRoll > 5){
              display.fillCircle(52, 36, 4, BLACK);
              display.display();
              delay(1000);
            }
          }
        }
      }
    }
  }
  else{
    //nothing here,  it's impossible to mess up
  }
    delay(1000);  
    display.clearDisplay();
    digitalWrite(led, LOW);
    lcd.clear();
}
void loop() {

if (Serial.available() > 0) {

// Read user input
  String input = Serial.readStringUntil('\n');
  input.trim();

// Convert input into integer
  int value = input.toInt();

// STEP 1
  if (step == 1) {

    PIB = value;

      if (PIB >= 0 && PIB <= InitialPots) {
      PRB = InitialPots - PIB;

      Serial.print("You've put ");
      Serial.print(PIB);
      Serial.println(" pots at bay.");

      Serial.print("Pots Remaining: ");
      Serial.println(PRB);

      Serial.println("\nHow many would you like to put at sea?");
      step = 2;
    }

// STEP 2
    else if (step == 2) {

    POS = value;
      
    if (POS >= 0 && POS <= PRB) {
      int PRS = PRB - POS;

      Serial.print("You've put ");
      Serial.print(POS);
      Serial.println(" pots at sea.");

      Serial.print("Pots Remaining: ");
      Serial.println(PRS);

      Serial.println("\nDone.");
      step = 3;

    } else {
        Serial.println("Invalid number. Try again:");
      }
    }
  }
  int DiceRoll = random(1, 7);
  Outcome(DiceRoll);
  RollDice(DiceRoll);
}
// STEP 3
    else if (step == 3) {

    PurchasedPots = value;
    if (PurchasedPots >= 0) {
          int TotalPots = PRB + PurchasedPots;

          Serial.print("You've purchased: ");
          Serial.print(PurchasedPots);
          Serial.println(" pots.");

          Serial.print("Total Pots Now: ");
          Serial.println(TotalPots);

          Serial.println("n\ Process complete.");
          int step = 1;

        } else {
          Serial.println("Invalid number. Try again:");
        }
        delay(5000);
        int step = 1;
        Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); //clears the serial monitor by starting new lines (NEW THING LEARNT)
        Serial.println("How many would you like to put at bay?");
    }
  }
}
