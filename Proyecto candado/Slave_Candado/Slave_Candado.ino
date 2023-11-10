#include <SoftwareSerial.h>
#include "LowPower.h"

// Use pin 2 as wake up pin



#define bateria 4 //Pin analogico para leer carga bateria
#define ledbatt 3
#define led 4 //Control de motor atras
#define m_abierto 5//Control de motor adelante
#define fin_de_carrera 6 //Para detener el motor
#define comienzo_de_carrera 7 //Para iniciar motor
const int wakeUpPin = 2;
int analogValor=0; //Donde se guarda le valor de la bateria
float voltaje=0;
int ledDelay=20;
unsigned long currentMillis=0;
unsigned long previousMillis = 0; //Variable de tiempo
const long interval = 10000; //Tiempo en ms, tiempo actual 60 segundos
int dato=0,i=0, posicion_m=0;
float maximo=4.9;
float minimo=4.0;
SoftwareSerial miBT(10,11);

void setup() {
  pinMode(wakeUpPin, INPUT); 
  pinMode(ledbatt, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(m_abierto,OUTPUT);
  pinMode(fin_de_carrera, INPUT);
  digitalWrite(led,LOW);
  
  miBT.begin(9600);//Velocidad de comunicación modulo blueth
  

}

void loop() {
 
 //attachInterrupt(0, wakeUp, LOW);
  if (miBT.available()>0){
    dato=miBT.read();
    
  }
  if (dato== '0'){
    
    
    int cerrado=digitalRead(fin_de_carrera);//pin6
    int abierto=digitalRead(comienzo_de_carrera);//pin7
    
    if (cerrado==LOW&&abierto==HIGH){ //cerradura abierta
      posicion_m=0;
    
      digitalWrite(m_abierto,LOW); //Activa motor para cerrar
      delay(5);
      digitalWrite(led,HIGH); //Activa motor para cerrar
      delay(5);
      while (cerrado==LOW){
        cerrado=digitalRead(fin_de_carrera);
      }
      digitalWrite(led,LOW);
      delay(5);
    }
    
    else{ //Cerradura cerrada
      posicion_m=1;
      digitalWrite(led,LOW); //Activa motor para abrir
      delay(5);
      digitalWrite(m_abierto,HIGH); //Activa motor para abrir
      delay(5);
      while (abierto==LOW){
        abierto=digitalRead(comienzo_de_carrera);
      }
      digitalWrite(m_abierto,LOW); //Activa motor para cerrar
      delay(5);
      
    }
    
    while(i==0){
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);   //// Enter power down state with ADC and BOD module disabled.
      i=1;
    }
    
  }
  }
 //detachInterrupt(0); 

 
 
 //Lectura de baterias
  analogValor=analogRead(bateria);// Lee voltaje de bateria
  voltaje=0.0048*analogValor;
  if (voltaje<minimo){
    digitalWrite(ledbatt,HIGH);
    delay(ledDelay);
  }
  else{
    digitalWrite(ledbatt,LOW);
    delay(ledDelay);
  }
  digitalWrite(led,LOW);
  delay(ledDelay);
 
      
  // put your main code here, to run repeatedly:

}
