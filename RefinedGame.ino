#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Wire.h>;

const int led = 13;
int outcome;
int DiceRoll;
int InitialPots = 10;
int PurchasedPots = 0;
int PIB = -1;
int PRB = 0;
int POS = -1;
int step = 1;
int Money = 0;
int PotCost = 5;
int TotalPots = InitialPots;

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
  Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); //clears the serial monitor by starting new lines (NEW THING LEARNT)
  Serial.println("You have 10 pots.");
  Serial.println("How many would you like to put at bay?");
  Update();
}

void Outcome(int DiceRoll) {
  lcd.clear();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.print("You Rolled: ");
  display.print(DiceRoll);
  display.display();
  int outcome = DiceRoll;
  if (outcome < 5) {
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
  else if (outcome > 6) { //If Dice Rolls a 6
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
  delay(2500);
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

void loop() {

if (Serial.available() > 0) {

    String input = Serial.readStringUntil('\n'); //Read Full line
    input.trim(); // remove spaces/newlines

    int value = input.toInt(); // convert to int

// STEP 1
    if (step == 1) {

    PIB = value;

      if (PIB >= 0 && PIB <= TotalPots) {
      PRB = TotalPots - PIB;

      Serial.print("You've put ");
      Serial.print(PIB);
      Serial.println(" pots at bay.");

      Serial.print("Pots Remaining: ");
      Serial.println(PRB);

      Serial.println("\nHow many would you like to put at sea?");
      Update();
      step = 2;

    } else {
    Serial.println("Invalid number. Try again:");
    }
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

    } else {
        Serial.println("Invalid number. Try again:");
      }
      int DiceRoll = random(1, 7);
      //int DiceRoll = 6;
      Outcome(DiceRoll);
      RollDice(DiceRoll);
      Update();
      if (TotalPots == 0 && Money <= 0){
        Serial.println("You have no more pots or money: GAME OVER!");
        step = 0;
      }
    }
  }

  else if (step == 3) {

    lcd.clear();

    Serial.println("Total Pots are displayed on the left");
    Serial.println("The total money is displayed on the right");

    Update();

    Serial.print("Each pot costs $");
    Serial.println(PotCost);

    Serial.println("\n");

    Serial.println("How many pots would you like to buy?");

    while (Serial.available() == 0) {
    // wait for input
    }

      String input = Serial.readStringUntil('\n');
      input.trim();

      PurchasedPots = input.toInt();

      int TotalCost = PurchasedPots * PotCost;

      if (PurchasedPots >= 0 && TotalCost <= Money) {

      Money -= TotalCost;
      TotalPots += PurchasedPots;

      Serial.print("You bought ");
      Serial.print(PurchasedPots);
      Serial.println(" pots.");

      Update();

      Serial.println("\n--- NEXT ROUND ---");

      step = 1;

      Serial.println("How many pots would you like to put at bay?");
      }

      else {

      Serial.println("Not enough money or invalid amount.");
    }
  }
}
