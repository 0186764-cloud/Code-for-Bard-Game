int InitialPots = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("You have 10 pots, how many would you like to put at bay?");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0){

    int PIB = Serial.parseInt(); //PIB stands for Pots in Bay

    if (PIB >= 0 && PIB <= InitialPots){
      int PRB = InitialPots - PIB;

      Serial.print("You've put ");
      Serial.println(PIB);      
      Serial.print("pots at bay.");

      Serial.print("Pots Remaining:");      
      Serial.println(PRB);
    }
    else {
      Serial.println("Invalid Number");
      delay(1000);
    }

    Serial.println("How many would you like to put at sea?");
    if (Serial.available() > 0){
      int POS = Serial.parseInt();
      int PRB = InitialPots - PIB;
      int PRS = PRB - POS;
        if (POS >= 0 && POS <= PRB){
          Serial.print("You've put ");
          Serial.println(PIB);      
          Serial.print("pots at sea.");
          Serial.print("Pots Remaining:");      
          Serial.println(PRS);
        }
        else {
          Serial.println("Invalid Number");
        }
        delay(2000);
      }
    }
  }
