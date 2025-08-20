#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>
#include <I2CKeyPad.h>

#include <LiquidCrystal_I2C.h>
#define I2C_ADDR 0x20
LiquidCrystal_I2C lcd(0x27,16,2);
I2CKeyPad keypad(I2C_ADDR);
  char keyMap[13] = "123456789*0#";
  
  const long delais = 500;
  unsigned long tempsPreced = 0;
unsigned long tempsActu;

    int temps = 2000;
    String texte;
    
    #define ENA D5
    #define IN1 D6
    #define IN2 D7
    #define IN3 D0
    #define IN4 D8
    #define ENB D3


    #define CODE_LENGTH 4
    const char codeSecret[CODE_LENGTH] = {'1', '2', '3', '4'}; // Exemple de code à 4 chiffres
    char saisie[CODE_LENGTH + 1]; // +1 pour '\0'
    uint8_t indexSaisie = 0;

//Servo moteur
    Servo porte1;
    Servo porte2;
    int motor1Pin = 3;
    int motor2Pin = 1;
    
//VITESSE MAXIMALE DES MOTEURS
    int speed = 255;
    
     #define bp1Pin D4
     #define bp2Pin A0

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);

  //-----------------Initialiser l'I2C avec le clavier--------------/
  Wire.begin(D2, D1);
  I2Cinit();
  //---------------------------------LCD INIT --------------------------------------------------//
  lcd.init(); 
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Init systeme... ");
//---------------------------------------init broche like input ---------------------------------------//
 
  pinMode(bp2Pin, INPUT_PULLUP);
  
//--------------------------------INITIALISATION DES BROCHES du driver-------------------------------//
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  //-------------------Reglage vitesse du moteur cc et 
  analogWriteRange(255);  //ECHELLE PWM
  analogWriteFreq(1000);  //FREQUENCE PWM(1kHZ)

  //Servo moteur
  porte1.attach(motor1Pin);
  porte2.attach(motor2Pin);
  porte1.write(0);
  porte2.write(0);
  delay(2000);
  

}

void loop() {
  // put your main code here, to run repeatedly:

  int bp2 = analogRead(bp2Pin);
  int bp1 = digitalRead(bp1Pin);
  if (bp1 == 0)
  {
    lcd.clear();
    openDoor(porte1,1);
    delay(100);
    legOut(1);
    delay(20);
    lcdPrint(texte ="Veillez mettre votre colis dans la barque",temps);
    
    lcdPrint(texte ="masse:2k Prix:5000f",temps);

    lcdPrint(texte ="select payout m",temps);

    lcdPrint(texte ="entrez votre id",temps);
 
    lcdPrint(texte ="confirmez sur le paiement..",temps);

    lcdPrint(texte ="Effectuer",temps);
    delay(10);
    
    legIn(1);
    
   lcdPrint(texte = "Pour rétirer votre colis veillez entrez le code suivant: 1234",temps);
    delay(200);
    closeDoor(porte1,1);
    
  }
  if (bp2 <= 500 )
  {
    lcdPrint(texte = "Veillez entrez le code reçu au dépot de colis ", temps);

    //char apuis = getKeypadValue();
    verifierCode();

    
    delay(200);
    lcd.clear();
    delay(10);
    lcdPrint(texte = "Votre colis est prêt ",temps);
    delay(200);
    openDoor(porte2,2);
    legOut(2);
    delay(1000);
    legIn(2);
    closeDoor(porte2,2);
    lcd.clear();
  }
}




void legOut(int motor)
{
  if (motor == 1){
  //SENS 1---ROTATION PENDANT 20s
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  }
  else{
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  }

  for (int i = 50; i <= speed; i += 100) {
    analogWrite(ENA, i);
    analogWrite(ENB, i);
    delay(50);
  }
  delay(20000);
}

void legIn(int motor)
{
  if (motor ==1){
  //SENS 2---ROTATION PENDANT 20s
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  }
  else{
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  }
  for (int i = 50; i <= speed; i += 100) {
    analogWrite(ENA, i);
    analogWrite(ENB, i);
    delay(50);
  }
  delay(20000);
}

void lcdPrint(String texte, int t)
  {
    delay(20);
    lcd.clear();
    delay(70);
    lcd.print(texte);
    delay(t);
    
    
  }


  void openDoor( Servo moteur, int n)
{
   for (int pos = 0; pos <= 180; pos += 5) {
    moteur.write(pos);
    if (n==2){moteur.write(180- pos);}
    moteur.write(180 - pos);  // inverse, juste pour voir les deux bouger différemment
    delay(20);
  }
}

void closeDoor( Servo moteur,int n)
{
  for (int pos = 180; pos >= 0; pos -= 5) {
    moteur.write(pos);
    if (n==2){moteur.write(180- pos);}
    delay(20);

  }
}

 void I2Cinit()
 {
    Serial.println("Scan I2C...");
  byte count = 0;
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print(" trouvé à l'adresse 0x");
      Serial.println(addr, HEX);
      count++;
    }
  }
  if (count == 0) {
    Serial.println("⚠️ Aucun périphérique I2C détecté !");
  }

 
  if (!keypad.begin()) {
    Serial.println("PCF8574 non détecté.");
  } else {
    keypad.loadKeyMap(keyMap);
    Serial.println("Keypad prêt.");
  }
 }


 void verifierCode() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Entrez code:");
  indexSaisie = 0;
  memset(saisie, 0, sizeof(saisie)); // Remise à zéro

  while (1) {
    if (keypad.isPressed()) {
      uint8_t raw = keypad.getKey();
      if (raw < 12) { // Touche valide sur 4x3 (indices 0 à 11)
        char c = keyMap[raw];

        // Accepte seulement chiffres, ignore les autres (A,B,C,D,*,#)
        if (c >= '0' && c <= '9') {
          if (indexSaisie < CODE_LENGTH) {
            saisie[indexSaisie++] = c;
            lcd.setCursor(indexSaisie-1, 1);
            lcd.print('*'); // Affiche un astérisque pour chaque chiffre saisi
          }
        }
        // Si touche '#' utilisée comme "Valider"
        else if (c == '#' && indexSaisie == CODE_LENGTH) {
          saisie[CODE_LENGTH] = '\0'; // Termine la chaîne
          bool correct = true;
          for (uint8_t i = 0; i < CODE_LENGTH; i++) {
            if (saisie[i] != codeSecret[i]) {
              correct = false;
              break;
            }
          }
          lcd.clear();
          if (correct) {
            lcd.print("Code OK !");
          } else {
            lcd.print("Code faux !");
          }
          delay(2000);
          break; // Quitte la fonction après vérification
        }
        // Si touche '*' utilisée comme "Effacer"
        else if (c == '*') {
          if (indexSaisie > 0) {
            indexSaisie--;
            saisie[indexSaisie] = '\0';
            lcd.setCursor(0,1);
            lcd.print("    "); // Efface la ligne de saisie
            lcd.setCursor(0,1);
            for (uint8_t j = 0; j < indexSaisie; j++) lcd.print('*');
          }
        }
        // Anti-rebond (attend relâchement)
        while (keypad.isPressed()) delay(50);
      }
    }
  }
}
  


