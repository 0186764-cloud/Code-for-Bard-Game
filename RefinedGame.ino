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
const int led = 13; //relates to the rin connected to the LED
int outcome; //Outcome from DiceRoll
int DiceRoll; //Signifies DiceRoll
int InitialPots = 10; //Pots given at the start
int PurchasedPots = 0; //Pots purchased
int PIB = -1; //Pots in Bay
int PRB = 0; //Pots remaining after placed in bay
int POS = -1; //Pots in Sea
int step = 1; //Step-byStep counter
int Money = 0; //Money
int PotCost = 5; //Cost of Pots
int TotalPots = InitialPots; //For later rounds

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
  Update();
}

void Outcome(int DiceRoll) {
  lcd.clear();
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

void RollDice(int DiceRoll){
  int xPos[6] = {32, 52, 32, 52, 32, 52};
  int yPos[6] = {12, 12, 24, 24, 36, 36};

  for (int i = 0; i < DiceRoll; i++) {
    display.fillCircle(xPos[i], yPos[i], 4, BLACK);

    display.display();

    delay(1000);
  }
}

void Update() { //Updates Money and Pot counter
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pots:");
    lcd.setCursor(0, 1);
    lcd.print(TotalPots);
    lcd.setCursor(7, 0);
    lcd.print("|");
    lcd.setCursor(7, 1);
    lcd.print("|");    
    lcd.setCursor(10, 0);
    lcd.print("Total:");
    lcd.setCursor(10, 1);
    lcd.print("$");
    lcd.setCursor(11, 1);
    lcd.print(Money);
}

void loop() { //RUNS INFINITELY
  
if (Serial.available() > 0) {

    String input = Serial.readStringUntil('\n'); //Read Full line
    input.trim(); // remove spaces/newlines

    int value = input.toInt(); // convert to int

// STEP 1
    if (step == 1) {

    PIB = value;  //relates to int value

      if (PIB >= 0 && PIB <= TotalPots) {//prevents invalid inputs
      PRB = TotalPots - PIB;

      Serial.print("You've put ");
      Serial.print(PIB);
      Serial.println(" pots at bay."); //Displays on Serial Monitor

      Serial.print("Pots Remaining: ");
      Serial.println(PRB);

      Serial.println("\nHow many would you like to put at sea?");
      Update();
      step = 2; //Initiates step 2

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

      Serial.println("\nDone."); //Adds spacing between lines

    } else {
        Serial.println("Invalid number. Try again:");
      }
      int DiceRoll = random(1, 7); //Randomises the value of DiceRoll
      //int DiceRoll = 6;
      Outcome(DiceRoll); //Displays the Outcome
      RollDice(DiceRoll); //Starts the Dice Rolling
      Update();
       if (TotalPots == 0 && Money <= 0){ //Checks if player doesn't have any more money or pots to use.
        Serial.println("You have no more pots or money: GAME OVER!"); //Ultimately stops the game
        step = 0; //stops the step process entirely.
      }
    }
  }

  else if (step == 3) {

    lcd.clear();

    Serial.println("Total Pots are displayed on the left");
    Serial.println("The total money is displayed on the right");

    Update(); ////Displays Pots remaining and total money after DiceRoll

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

      Update();

      Serial.println("\n--- NEXT ROUND ---"); //Proceeds to Next Round by creating a new line

      step = 1; //Resets the step process

      Serial.println("How many pots would you like to put at bay?"); //Asks the same question given at the start
      }

      else {

      Serial.println("Not enough money or invalid amount."); //Invalid amount
    }
  }
}
