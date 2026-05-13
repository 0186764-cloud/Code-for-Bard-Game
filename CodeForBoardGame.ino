//Explanation of the Game:
//This game is called Crayfish. A game of resource management and chance. Inspired by a game I played in primary school, 
//The player is trying to make the most money out of the competition. You place pots in either the BAY or the SEA. 
//Depending on the role, you would either gain money or lose your pots. Pots may be purchased at the end of the round if 
//the player desires to. 
//(ONLY single player for now) - Inspired by Russian Roulette

#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Wire.h>;
//LIBRARY INITIALISATION - Imports Libraries so hardware can function

//DECLARES VARIABLES
const int led = 13; //Relates to the pin connected to the LED
int outcome; //Outcome from diceroll
int DiceRoll; //Signifies Dice Roll
int InitialPots = 10; //Pots given at the start of the game
int PurchasedPots = 0; //Pots purchased
int PIB = -1; //Pots in bay
int PRB = 0; //Pots Remaining after placed in bay
int POS = -1; //Pots in Sea
int step = 1; //Step-by-Step counter
int Money = 10; //Money
int PotCost = 2; //Cost of Pots
int TotalPots = InitialPots; //At the start, both are the same(For later rounds).

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
LiquidCrystal_I2C lcd(0x27, 16, 2);
//Initialise two different types of displays by assigning specific communication parameters to  control in the sketch.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Starts Serial Monitor with a 9600 baud rate
  pinMode(led, OUTPUT); //Creates the LED as an OUTPUT
  lcd.init(); //Powers LCD Screen on
  lcd.backlight();
  display.begin(); //Turns on Nokia LCD Module

  display.setContrast(50); //Sets contrast of backlight compared to the text
  display.clearDisplay(); //Clears display before use
  Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); //clears the serial monitor by starting new lines
  Serial.println("You have 10 pots.");
  Serial.println("How many would you like to put at bay?"); //Asks question
}

void Outcome(int DiceRoll) { //Outcome calculation process
  display.setTextSize(1); //Sets text size
  display.setTextColor(BLACK); //Sets text colour
  display.setCursor(0, 0); //Sets text position
  display.print("You Rolled: ");
  display.print(DiceRoll); //Displays value on Nokia Screen
  display.display();
  int outcome = DiceRoll;
  if (outcome < 4) { //If Dice Rolls a 1, 2 or a 3.
    lcd.setCursor(6, 0);
    lcd.print("It's");
    lcd.setCursor(6, 1);
    lcd.print("Calm"); //Presents Outcome
    delay(1000); //Less delay required
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
    delay(1000); //Less delay required
    int Loss = POS; //Lose pots
    TotalPots -= Loss; //Lose pots + 0 money gain
    Serial.print("Bad weather! Lost ");
    Serial.println(Loss);
    Serial.print("pots");
  } 
  else { //If the dice rolls either a 4 or 5
    lcd.setCursor(6, 0);
    lcd.print("It's");
    lcd.setCursor(5, 1);
    lcd.print("A Storm"); //Presents Outcome using LCD Screen
    delay(1000); //Less delay required
    int earnings = PIB*2 + (POS * 4); //Earn but higher reward
    Money += earnings;
    Serial.print("Successfull trip! Earned $");
    Serial.println(earnings);
  }
  delay(2500); //Less delay required
  step = 3;
  Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); //clears the serial monitor by starting new lines (NEW THING LEARNT)
  //Serial.println("How many would you like to put at bay?");
}
void RollDice(int DiceRoll){ //DICE ANIMATION - Visually displays the dice results on the Nokia Screen.
  if (DiceRoll > 0){ //nested if statements (Further Improvement?)
    display.fillCircle(32, 12, 4, BLACK);
    display.display();
    delay(1000); //Less delay required

    if (DiceRoll > 1){
      display.fillCircle(52, 12, 4, BLACK);
      display.display();
      delay(1000); //Less delay required

      if (DiceRoll > 2){
        display.fillCircle(32, 24, 4, BLACK);
        display.display();
        delay(1000); //Less delay required

        if (DiceRoll > 3){
          display.fillCircle(52, 24, 4, BLACK);
          display.display(); 
          delay(1000); //Less delay required

          if (DiceRoll > 4){
            display.fillCircle(32, 36, 4, BLACK);
            display.display();
            delay(1000); //Less delay required

            if (DiceRoll > 5){
              display.fillCircle(52, 36, 4, BLACK);
              display.display();
              delay(1000); //Less delay required
            }
          }
        }
      }
    }
  }
  else{
    //nothing here,  it's impossible to mess up
  }
    Serial.println("Wait a few seconds..."); //Reminder for player not to meddle while calculating
    delay(2500); //Maybe remove? 
    display.clearDisplay();
    digitalWrite(led, LOW); //turns the LED off after a 6 is rolled.
    lcd.clear();
}

void loop() { //RUNS INFINITELY

if (Serial.available() > 0) {//checks if player typed something

    String input = Serial.readStringUntil('\n'); //Read Full line
    input.trim(); // remove spaces/newlines

    int value = input.toInt(); // convert to int

// STEP 1 - BAY
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

// STEP 2 - SEA
  else if (step == 2) {

  POS = value;

    if (POS >= 0 && POS <= PRB) {//Similar to last time, validates for invalid inputs
      int PRS = PRB - POS; //deducts Remaining pots from 

      Serial.print("You've put ");
      Serial.print(POS);
      Serial.println(" pots at sea.");

      Serial.print("Pots Remaining: ");
      Serial.println(PRS);

      Serial.println("\nDone."); //Adds spacing between lines

    } else {
        Serial.println("Invalid number. Try again:");
      }
      int DiceRoll = random(1, 7); //Randomises the value of DiceRoll
      //int DiceRoll = 6;
      Outcome(DiceRoll); //Displays the Outcome
      RollDice(DiceRoll); //Starts the Dice Rolling
      if (TotalPots == 0 && Money <= 0){ //Checks if player doesn't have any more money or pots to use.
        Serial.println("You have no more pots or money: GAME OVER!"); //Ultimately stops the game
        step = 0; //stops the step process entirely.
      }
    }
  }
//STEP 3 - SHOP
  else if (step == 3) {

    Serial.print("You currently have ");
    Serial.println(TotalPots);
    Serial.println("pots"); //Displays Pots remaining after DiceRoll

    Serial.println("\n"); //Adds spacing between lines

    Serial.print("You currently have: $");
    Serial.println(Money); //Displays total money before using shop.

    Serial.println("\n"); //Adds spacing between lines

    Serial.print("Each pot costs $");
    Serial.println(PotCost); //Displays Pot cost

    Serial.println("\n"); //Adds spacing between lines

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

      Serial.println("\n--- NEXT ROUND ---"); //Proceeds to Next Round by creating a new line

      step = 1; //Resets the step process

      Serial.println("How many pots would you like to put at bay?"); //Asks the same question given at the start
      }

      else {

      Serial.println("Not enough money or invalid amount."); //Invalid amount
    }
  }
}
