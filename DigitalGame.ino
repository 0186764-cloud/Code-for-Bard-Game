int InitialPots = 10;
int PIB = 0;
int PRB = 0;
int POS = 0;

int Question = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("You have 10 pots, how many would you like to put at bay?");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0){

    if (Question == 1){
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
    }
    delay(2000);
    Question = 2;
    Serial.println("How many would you like to put at sea?");
    }
    else if (Question == 2){
      POS = Serial.parseInt();
      if (POS >= 0 && POS <= PRB){
        int PRS = PRB - POS;
        Serial.print("You've put ");
        Serial.println(POS);      
        Serial.print("pots at sea.");

        Serial.print("Pots Remaining:");      
        Serial.println(PRS);

        Serial.println("\nDone.");
        Question = 3;
      }
      else {
        Serial.println("Invalid Number");
      }
      delay(2000);
    }    
  }
}
