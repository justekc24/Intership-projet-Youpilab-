#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>
#include <I2CKeyPad.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4); 
#define I2C_ADDR 0x20
I2CKeyPad keypad(I2C_ADDR);
const char keyMap[19] = "123456789*0#"; 

#define ENA D1
#define IN1 D2
#define IN2 D3
#define IN3 D4
#define IN4 D5
#define ENB D6
//VITESSE MAXIMALE DES MOTEURS
int speed = 255;

Servo porte1;
Servo porte2;
int motor1Pin = D7;
int motor2Pin = D8;

//char valeurs[10];   // Tableau pour stocker les valeurs
//char indexTableau = 0; // Position actuelle dans le tableau

/* A supprimer
 *  //Pour le PF8475
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
*/


 #define bp1Pin A0
 #define bp2Pin D4

void setup() {
  // put your setup code here, to run once:
  //-----------------Initialiser l'I2C avec le clavier--------------/
  Wire.begin(3,1); 
  Serial.begin(115200);
  keypadInit();
  
  //---------------------------------LCD INIT --------------------------------------------------//
  lcd.init(); 
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Init systeme... ");
//---------------------------------------init broche like input ---------------------------------------//
  pinMode(bp1Pin, INPUT);
  pinMode(bp2Pin, INPUT);
  
//--------------------------------INITIALISATION DES BROCHES du driver-------------------------------//
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  analogWriteRange(255);  //ECHELLE PWM
  analogWriteFreq(1000);  //FREQUENCE PWM(1kHZ)
  porte1.attach(motor1Pin);
  porte2.attach(motor2Pin);
  porte1.write(0);
  porte2.write(0);
  

}

void loop() {
  // put your main code here, to run repeatedly:

  int bp1 = digitalRead(bp1Pin);
  int bp2 = digitalRead(bp2Pin);
  if (bp1 == 1)
  {
    lcd.clear();
    openDoor(porte1);
    delay(1000);
    legOut(1);
    lcd.print("Veillez mettre votre colis dans la barque");
    delay(200);
    lcd.clear();
    delay(10);
    lcd.print ("La masse de votre colis est 12k correspondanr à une somme de 5000f");
    delay(200);
    lcd.clear();
    delay(10);
    lcd.print("veillez selectionnenez votre mode de paiement ");
    delay(200);
    lcd.clear();
    delay(10);
    lcd.print("Veillez entre votre numero bancaire / veillez entrez votre numero de paiement(+2290");
    delay(200);
    lcd.clear();
    delay(10);
    lcd.print("confirmez sur le paiement..");
    delay(200);
    lcd.clear();
    delay(10);
    lcd.print("Transfert effectué");
    delay(200);
    lcd.clear();
    delay(10);
    legIn(1);
    lcd.clear();
    delay(10);
    lcd.print("Pour rétirer votre colis veillez entrez le code suivant: 1234");
    delay(200);
    closeDoor(porte1);
    
  }
  if (bp2 == 1)
  {
    lcd.clear();
    delay(10);
    lcd.print("Veillez entrez le code reçu au dépot de colis ");

    char apuis = getKeypadValue();
    if (apuis == 3)
    {
      
    

 /* Tout ce qui concerne l'entrer du cde lors du retrait valider avant d'ouvrir la porte   
    bool enter = true;
    while (enter)
    {
      char apuis = getKeypadValue()
       if (apuis >= 0 && apuis <= 9) {
        if (indexTableau < 3) { // Empêche de dépasser le tableau
          valeurs[indexTableau] = apuis;
          Serial.print("Valeur acceptée et stockée : ");
          Serial.println(variable);
          lcd.clear();
          delay(10);
          lcd.setCursor(indexTableau,1);
          lcd.print(apuis)
          indexTableau++;
        } else {
          Serial.println("Tableau plein !");
        }
      } else {
        Serial.print("Valeur rejetée : ");
        Serial.println(variable);
      }
      
    }

*/
    
    delay(200);
    lcd.clear();
    delay(10);
    lcd.print("Votre colis est prêt ");
    delay(200);
    openDoor(porte2);
    legOut(2);
    delay(2000);
    legIn(2);
    closeDoor(porte2);
    
  }
  }
}




void openDoor( Servo moteur)
{
   for (int i = 0; i <= 90; i++) {
      moteur.write(i);
      delay(15);
    }
}

void closeDoor( Servo moteur)
{
  for (int i = 90; i <= 0; i--) {
      moteur.write(i);
      delay(15);
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
  
  



//Fonction pour recureper et initialiser le clavier avec le PF8574 

 char getKeypadValue()
 {
  char c = keypad.getChar();
  if (c != I2C_KEYPAD_NOKEY && c != I2C_KEYPAD_FAIL && c != I2C_KEYPAD_THRESHOLD) {
    Serial.print("Touche: ");
    Serial.println(c);
    
  }
  return c;
 };
 

 void keypadInit()
 {
  while (!Serial);

  if (!keypad.begin()) {
    Serial.println("Erreur: PCF8574 non détecté");
    while (1);
  }
  keypad.loadKeyMap((char*)keyMap);
  Serial.println("Keypad prêt");
 
 }
