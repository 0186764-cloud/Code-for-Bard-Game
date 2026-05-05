int InitialPots = 10;

int PIB = -1;
int PRB = 0;
int POS = -1;

int step = 1;

void setup() {
  Serial.begin(9600);
  Serial.println("You have 10 pots.");
  Serial.println("How many would you like to put at bay?");
}

void loop() {

  if (Serial.available() > 0) {

    String input = Serial.readStringUntil('\n'); // ✅ read full line
    input.trim(); // remove spaces/newlines

    int value = input.toInt(); // convert to int

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
      step = 3;

    } else {
        Serial.println("Invalid number. Try again:");
      }
    }
  }
}
