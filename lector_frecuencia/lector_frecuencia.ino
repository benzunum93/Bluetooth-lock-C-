#include <LiquidCrystal.h>
volatile unsigned long t0=0;
volatile unsigned long tn=0;
volatile unsigned long deltaT=0;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 7, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
double frecuencia=0;

void setup() {
  Serial.begin(9600);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(8,OUTPUT);
   pinMode(A0,OUTPUT);
  attachInterrupt(0,periodo,RISING);
 
lcd.begin(16, 2);
  // Print a message to the LCD.
  
}

void loop() {
  frecuencia=1000/deltaT;
   lcd.setCursor(0, 1);
  lcd.print(frecuencia);
   
  delay(200);
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print(" Pulsos por min");
  if (frecuencia<10){
    while (frecuencia<10){
       digitalWrite(8,HIGH);
       delay(1000);
      
      digitalWrite(8,LOW);
       delay(1000);
      frecuencia=1000/deltaT;
      interrupts();
     
    lcd.print("Frecuencia baja");
    delay(200);
    lcd.clear();
    }
    
   interrupts(); 
  }
  else{
  digitalWrite(8, HIGH); 
  
  }

}

void periodo()
{
  noInterrupts();
  tn=millis();
  deltaT=tn-t0;
  t0=tn;
}
