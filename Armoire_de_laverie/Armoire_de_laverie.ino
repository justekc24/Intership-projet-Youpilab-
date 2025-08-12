#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Pour le PF8475
#define PCF8574_ADDR 0x20 
byte pcfState = 0xFF; // Toutes les broches en HIGH au démarrage (entrée ou sortie HIGH)
// Définition du clavier
const byte ROWS = 4; // 4 lignsses
const byte COLS = 3; // 3 colonnes

LiquidCrystal_I2C lcd(lcdAddress, lcdColumns, lcdRows);

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
//--------------------------Connecté aux broches du clavier (lignes) et (colonnes)----------------
byte rowPins[ROWS] = {16, 5, 4, 0};     
byte colPins[COLS] = {2, 14, 12};   

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

 int bp1Pin =  12;
 int bp2Pin = 13;

void setup() {
  // put your setup code here, to run once:
  Wire.begin(); // Pour le PF8475
  //---------------------------------LCD INIT --------------------------------------------------//
  lcd.init(); 
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Init systeme... ");
//---------------------------------------end ---------------------------------------//
  pinMode(bp1Pin, INPUT);
  pinMode(bp2Pin, INPUT);
  
  

}

void loop() {
  // put your main code here, to run repeatedly:

  int bp1 = digitalRead(bp1Pin);
  int bp2 = digitalRead(bp2Pin);
  if (bp1 == 1)
  {
    //lcd.print("Veillez mettre votre colis dans la barque");
    //opendoor1();
    //legout1();
    //lcd.print ("La masse de votre colis est 12k correspondanr à une somme de 5000f");
    //lcd.print("veillez selectionnenez votre mode de paiement ");
    //lcd.print("Veillez entre votre numero bancaire / veillez entrez votre numero de paiement(+2290");
    //lcd.print("confirmez sur le paiement..");
    //lcd.print("Transfert effectué");
    //legIn1);
    
  }
  if (bp2 == 1)
  {
    //lcd.print("Veillez mettre votre colis dans la barque");
    //opendoor1();
    //legout1();
    //lcd.print ("La masse de votre colis est 12k correspondanr à une somme de 5000f");
    //lcd.print("veillez selectionnenez votre mode de paiement ");
    //lcd.print("Veillez entre votre numero bancaire / veillez entrez votre numero de paiement(+2290");
    //lcd.print("confirmez sur le paiement..");
    //lcd.print("Transfert effectué");
    //legIn1);
    
  }
}

void openDoor()
{
   for (int i = 0; i <= 90; i++) {
      porte.write(i);
      delay(15);
    }

}

void closeDoor()
{
  
}

void legOut()
{
  
}

void legIn()
{
  
}
  
  
void motorActive()
{
  // Mettre P2 à HIGH
  bitSet(pcfState, 2); // P2 = 1
  Wire.beginTransmission(PCF8574_ADDR);
  Wire.write(pcfState);
  Wire.endTransmission();
  delay(1000);

  // Mettre P2 à LOW
  bitClear(pcfState, 2); // P2 = 0
  Wire.beginTransmission(PCF8574_ADDR);
  Wire.write(pcfState);
  Wire.endTransmission();
  delay(1000);
}



