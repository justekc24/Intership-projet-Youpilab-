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

byte rowPins[ROWS] = {16, 5, 4, 0};     // Connecté aux broches du clavier (lignes)
byte colPins[COLS] = {2, 14, 12};        // Connecté aux broches du clavier (colonnes)

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup() {
  // put your setup code here, to run once:
  Wire.begin(); // Pour le PF8475
  //---------------------------------LCD INIT --------------------------------------------------//
lcd.init(); 
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Init systeme... ");


}

void loop() {
  // put your main code here, to run repeatedly:

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




