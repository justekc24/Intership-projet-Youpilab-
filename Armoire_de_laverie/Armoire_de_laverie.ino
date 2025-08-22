#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>
#include <I2CKeyPad.h>

#include <LiquidCrystal_I2C.h>
#define I2C_ADDR 0x20
LiquidCrystal_I2C lcd(0x27,16,2);
I2CKeyPad keypad(I2C_ADDR);
  char keyMap[17] = "123A456B789C*0#D";
  
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
    int bp1; 
    int bp2;

    #define CODE_LENGTH 4
    const char codeSecret[CODE_LENGTH] = {'1', '2', '3', '4'}; 
    char saisie[CODE_LENGTH + 1]; // +1 pour '\0'
    uint8_t indexSaisie = 0;

//Servo moteur
    Servo porte1;
    Servo porte2;
    int motor1Pin = 1;
    int motor2Pin = 3;
    
//VITESSE MAXIMALE DES MOTEURS
    int speed = 255;
    
     #define bp1Pin D4
     #define bp2Pin A0
     volatile bool etatInterruption = false;
     void IRAM_ATTR boutonISR() {
      etatInterruption = true;   // Met un drapeau
    }

      


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
  lcdPrint(texte= " ", 10);
//---------------------------------------init broche like input ---------------------------------------//
 
  pinMode(bp2Pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(bp1Pin), boutonISR, FALLING);

  
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
  
  
  porte2.write(0);
  //delay(1000);
  porte1.write(180);
  delay(100);
  lcd.setCursor(0,0);
  lcd.print("RETRAIT");
  lcd.setCursor(0,1);
  lcd.print("DEPOT");

}

void loop() {
  // put your main code here, to run repeatedly:

   bp2 = analogRead(bp2Pin);
   if (etatInterruption) {
    etatInterruption = false;
    lcd.clear();
    openDoor(porte1,1);
 
    lcd.clear();
    delay(100);
    legIn(1);
    delay(20);
    lcdPrint(texte ="Mettez colis",temps);
    
    lcdPrint(texte ="M:2k Prix:5000f",temps);

    lcdPrint(texte ="select payout m",temps);

    lcdPrint(texte ="entrez num:",temps);
 
    lcdPrint(texte ="confirmez paiement..",temps);

    lcdPrint(texte ="Effectuer",temps);
    delay(10);
    lcd.clear();
    legOut(1);
    delay(1000);
   lcdPrint(texte = "codeRetrait:1234",temps);
   lcd.clear();
    delay(200);
    
    closeDoor(porte1,1);
    delay(100);
    lcd.clear();
    //ESP.restart();
   
    
  }
  else if (bp2 <= 500 )
  {
    lcdPrint(texte = "Entrez codeRecu", temps);
    delay(1000);
    //char apuis = getKeypadValue();
 
   verifierCode();
    //delay(200);
     

    
  // ESP.restart();
  }
  else{
    lcdPrint(texte= " ", 10);
    lcd.setCursor(0,0);
    lcd.print("RETRAIT");
    lcd.setCursor(0,1);
    lcd.print("DEPOT");
    
    delay(500);
  }
 
}




void legOut(int motor)
{
  if (motor == 1){
  //SENS 1---ROTATION PENDANT 20s
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  delay(1000);
  }
  else{
    if (motor == 2){
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  delay(1800);
    }
  }

  delay(300);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void legIn(int motor)
{
  if (motor ==1){
  //SENS 2---ROTATION PENDANT 20s
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  delay(4000);
  }
  else{
    if (motor == 2){
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  delay(1500);
  }}
  
    delay(50);
   
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void lcdPrint(String texte, int t)
  {
    
    lcd.setCursor(0, 0);   
    lcd.print("       ");        
    lcd.setCursor(0, 0); 
    lcd.setCursor(0, 1);   
    lcd.print("       ");        
    lcd.setCursor(0, 1);  

    delay(20);
    lcd.clear();
    delay(70);
    lcd.print(texte);
    delay(t);
    lcd.setCursor(0, 0);   
    lcd.print("       ");        
    lcd.setCursor(0, 0); 
    lcd.setCursor(0, 1);   
    lcd.print("       ");        
    lcd.setCursor(0, 1);  

    
  }


  void openDoor( Servo moteur, int n)
{
  if (n==1){
   for (int pos = 180; pos >= 0; pos -= 1) { 
    moteur.write(pos);                  
                             
  }
    
  }
  if (n==2){
   for (int pos = 0; pos <= 180; pos += 1) { 
    // in steps of 1 degree
    moteur.write(pos);  
    delay(15);           
  }
    
  }
  
}

void closeDoor( Servo moteur,int n)
{
  if (n==1){
    for (int pos = 0; pos <= 180; pos += 1) {  
    // in steps of 1 degree
    moteur.write(pos);  
              
  }
    }
    if (n==2){
    for (int pos = 180; pos >= 0; pos -= 1) {  
    moteur.write(pos);                  
    delay(15);                           
  }
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
    Serial.println("Aucun périphérique I2C détecté !");
  }

 
  if (!keypad.begin()) {
    Serial.println("PCF8574 non détecté.");
  } else {
    keypad.loadKeyMap(keyMap);
    Serial.println("Keypad prêt.");
  }
 }







void verifierCode() {
  uint8_t essais = 0; // Compteur d'erreurs

  while (essais < 3) {   // Tant qu'on n'a pas atteint 3 erreurs
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Entrez code:");
    indexSaisie = 0;
    memset(saisie, 0, sizeof(saisie)); // Remise à zéro

    while (1) {
      if (keypad.isPressed()) {
        uint8_t raw = keypad.getKey();
        if (raw < 16) { 
          char c = keyMap[raw];

          // --- Cas 1 : chiffre entré ---
          if (c >= '0' && c <= '9') {
            if (indexSaisie < CODE_LENGTH) {
              saisie[indexSaisie++] = c;
              lcd.setCursor(indexSaisie-1, 1);
              lcd.print('*');
            }
          }

          // --- Cas 2 : validation par '#' ---
          else if (c == '#' && indexSaisie == CODE_LENGTH) {
            saisie[CODE_LENGTH] = '\0'; 
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
      delay(2000);
      delay(10);
      lcd.clear();
      delay(10);
      lcdPrint(texte = "COLIS Pret",temps);
      delay(200);
      lcd.clear();
      openDoor(porte2,2);
      
      delay(1000);
      legIn(2);
      delay(2000);
      legOut(2);
      delay(2000);;
      //porte2.write(0);
      closeDoor(porte2,2);
      lcd.clear();
      
              return; // Sort immédiatement si code correct
              //ESP.restart(); 
              break;
            } else {
              essais++; // Incrémente le nombre d’erreurs
              lcd.print("Code faux !");
              delay(1000);
              lcd.clear();
              break; // Sort de la boucle interne et redemande code
            
            }
          }

          // --- Cas 3 : effacement avec '*' ---
          else if (c == '*') {
            if (indexSaisie > 0) {
              indexSaisie--;
              saisie[indexSaisie] = '\0';
              lcd.setCursor(0,1);
              lcd.print("    "); 
              lcd.setCursor(0,1);
              for (uint8_t j = 0; j < indexSaisie; j++) lcd.print('*');
            }
          }

          // Anti-rebond
          while (keypad.isPressed()) delay(50);
        }
      }
    }
  }

  // Si on arrive ici 3 erreurs consécutives
  lcd.clear();
  lcd.print("Acces bloque!");
  delay(1000);
  lcd.clear();
    //ESP.restart(); 
    //break;
}





