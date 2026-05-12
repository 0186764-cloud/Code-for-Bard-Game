#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Wire.h>;
//LIBRARY INITIALISATION - Imports Libraries so hardware can function

const int led = 13; //Relates to the pin connected to the LED
int outcome; //Outcome from diceroll
int DiceRoll; //Signifies Dice Roll
int InitialPots = 10; //Pots given at the start of the game
int PurchasedPots = 0; //Pots purchased
int PIB = -1; //Pots in bay
int PRB = 0; //Pots Remaining after placed in bay
int POS = -1; //Pots in Sea
int step = 1; //Step-to-Step
int Money = 10; //Money
int PotCost = 2; //Cost of Pots
int TotalPots = InitialPots; //At the start, both are the same.

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Starts Serial Monitor with 9600 baud rate
  pinMode(led, OUTPUT);
  lcd.init(); //Powers LCD Screen on
  lcd.backlight();
  display.begin(); //Turns on Nokia LCD Module

  display.setContrast(50);
  display.clearDisplay();
  Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); //clears the serial monitor by starting new lines (NEW THING LEARNT)
  Serial.println("You have 10 pots.");
  Serial.println("How many would you like to put at bay?"); //Asks question
}

void Outcome(int DiceRoll) { //Outcome calculation process
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.print("You Rolled: ");
  display.print(DiceRoll); //Displays value on Nokia Screen
  display.display();
  int outcome = DiceRoll;
  if (outcome < 4) { //If Dice Rolls a 1, 2 or a 3.
    lcd.setCursor(6, 0);
    lcd.print("It's");
    lcd.setCursor(6, 1);
    lcd.print("Calm"); //Presents Outcome
    delay(1000);
    int earnings = PIB + (POS * 2); //Earn money
    Money += earnings; //Added to total money
    Serial.print("Successfull trip! Earned $");
    Serial.println(earnings);
  } 
  else if (outcome > 5) { //If Dice Rolls a 6
    lcd.setCursor(6, 0);
    lcd.print("It's");
    lcd.setCursor(3, 1);
    lcd.print("A CALAMITY"); //Presents Outcome
    digitalWrite(led, HIGH); //Symbolises a Calamity or a 6
    delay(1000);
    int Loss = POS; //Lose pots
    TotalPots -= Loss; //Lose pots + 0 money gain
    Serial.print("Bad weather! Lost ");
    Serial.println(Loss);
    Serial.print("pots");
  } 
  else {
    lcd.setCursor(6, 0);
    lcd.print("It's");
    lcd.setCursor(5, 1);
    lcd.print("A Storm"); //Presents Outcome using LCD Screen
    delay(1000);
    int earnings = PIB*2 + (POS * 4); //Earn but higher reward
    Money += earnings;
    Serial.print("Successfull trip! Earned $");
    Serial.println(earnings);
  }
  delay(2500);
  step = 3;
  Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); //clears the serial monitor by starting new lines (NEW THING LEARNT)
  //Serial.println("How many would you like to put at bay?");
}
void RollDice(int DiceRoll){ //DICE ANIMATION - Visually displays the dice results on the Nokia Screen.
  if (DiceRoll > 0){ //nested if statements
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
    Serial.println("Wait a few seconds...");    
    delay(2500);  
    display.clearDisplay();
    digitalWrite(led, LOW);
    lcd.clear();
}

void loop() { //runs infinitely

if (Serial.available() > 0) {//checks if player typed something

    String input = Serial.readStringUntil('\n'); //Read Full line
    input.trim(); // remove spaces/newlines

    int value = input.toInt(); // convert to int

// STEP 1
    if (step == 1) {

    PIB = value; //relates to int value

      if (PIB >= 0 && PIB <= TotalPots) {//prevents invalid inputs
      PRB = TotalPots - PIB;

      Serial.print("You've put ");
      Serial.print(PIB);
      Serial.println(" pots at bay."); //Displays on Serial Monitor

      Serial.print("Pots Remaining: ");
      Serial.println(PRB);

      Serial.println("\nHow many would you like to put at sea?");
      step = 2; //Initiates Step 2

    } else {
    Serial.println("Invalid number. Try again:");
    }
  }

// STEP 2
  else if (step == 2) {

  POS = value;

    if (POS >= 0 && POS <= PRB) {//Similar to last time, validates for invalid inputs
      int PRS = PRB - POS; //deducts Remaining pots from 

      Serial.print("You've put ");
      Serial.print(POS);
      Serial.println(" pots at sea.");

      Serial.print("Pots Remaining: ");
      Serial.println(PRS);

      Serial.println("\nDone.");

    } else {
        Serial.println("Invalid number. Try again:");
      }
      int DiceRoll = random(1, 7);
      //int DiceRoll = 6;
      Outcome(DiceRoll); //Displays the Outcome
      RollDice(DiceRoll); //Starts the Dice Rolling
      if (TotalPots == 0 && Money <= 0){ //Checks if player doesn't have any more money or pots to use.
        Serial.println("You have no more pots or money: GAME OVER!"); //Ultimately stops the game
        step = 0; //stops the step process entirely.
      }
    }
  }
//STEP 3
  else if (step == 3) {

    Serial.print("You currently have ");
    Serial.println(TotalPots);
    Serial.println("pots"); //Displays Pots remaining after DiceRoll

    Serial.println("\n");    

    Serial.print("You currently have: $");
    Serial.println(Money); //Displays total money before using shop.

    Serial.println("\n");

    Serial.print("Each pot costs $");
    Serial.println(PotCost); //Displays Pot cost

    Serial.println("\n");

    Serial.println("How many pots would you like to buy?"); //Asks how many pots you would like to buy

    while (Serial.available() == 0) {
    // wait for input
    }

      String input = Serial.readStringUntil('\n');
      input.trim();

      PurchasedPots = input.toInt(); //Converts typed number into amount bought

      int TotalCost = PurchasedPots * PotCost; //Cost calculation

      if (PurchasedPots >= 0 && TotalCost <= Money) { //prevents overspending

      Money -= TotalCost; 
      TotalPots += PurchasedPots;
      //Calculations for purchases from the shop

      Serial.print("You bought ");
      Serial.print(PurchasedPots); //Displays the amount of pots purchased
      Serial.println(" pots.");

      Serial.print("Money Remaining: $");
      Serial.println(Money); //Displays money remaining

      Serial.print("Total Pots: ");
      Serial.println(TotalPots); //Displays total pots after purchase

      Serial.println("\n--- NEXT ROUND ---"); //Proceeds to Next Round

      step = 1; //Resets the step process

      Serial.println("How many pots would you like to put at bay?"); //Asks the same question given at the start
      }

      else {

      Serial.println("Not enough money or invalid amount."); //Invalid amount
    }
  }
}
