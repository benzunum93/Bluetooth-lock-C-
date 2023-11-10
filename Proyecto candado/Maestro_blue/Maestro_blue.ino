/***************************************************
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/


#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

#define batteria 4 //Entrada analogica donde mide la bateria
#define ledindicador 6 // Indicador del nivel de bateria
#define pulsador 4// activa los dispositivos, si se pulsa mas de un tiempo determinado entra en modo de resgistro de huellas
#define led 5 //Para indicaciones de funcionamiento generales, en un futuro se retirara
const int bt=12;
int id=0,huellero=0;
int addr=0;
int primera_vez=0;
int contador=0;
int id_dedo=0;
int repetido=100;
int analogValor=0; //Donde medimos la bateria
float voltaje=0;
int ledDelay=5;
float maximo=4.9;//maximo voltaje
float minimo=4.0;//bateria descargada
int cont=0;
SoftwareSerial miBT(7,8); //El primero es RX 
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..

SoftwareSerial mySerial(2, 3);

#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1

#endif


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  
  pinMode(8,OUTPUT);//TX hacia Bluetooth
  pinMode(11,OUTPUT);
  pinMode(led,OUTPUT);
  pinMode(bt, OUTPUT);
  pinMode(pulsador,INPUT);
  digitalWrite(bt,HIGH);
  pinMode(ledindicador, OUTPUT);
  Serial.begin(9600);
  miBT.begin(9600);//Velocidad de comunicación modulo blueth
  delay(5);
  //Serial.println("\n\nAdafruit finger detect test");
  finger.begin(57600);
  delay(5);
  
  while(huellero==0){ //El programa no avanza hasta que el huellero sea reconocido
        if (finger.verifyPassword()) {
              //Serial.println("Detectado un sensor de huella!");
            digitalWrite(ledindicador, HIGH);
            delay(200);
            digitalWrite(ledindicador, LOW);
            delay(10);
            huellero=1;
       }
       
  }
  //Primera huella
  primera_vez = EEPROM.read(0);
  
  if (primera_vez!=1){ //Primera huella que se enrolla
      EEPROM.write(0, 1);
      digitalWrite(led,HIGH);
      delay(5);
         
      contador++;
      getFingerprintEnroll();
      digitalWrite(led,LOW);
      delay(5);
  } 
  
}

void loop() {
  
  int valor;
  
  getFingerprintIDez();//Pide huellas
  delay(10);            //retardo de 10 milisegundos entre lecturas
  
  ///***Enrollar mas huellas con un pulsador*
  valor=digitalRead(pulsador);
  if(valor==HIGH){
    delay(200);
    valor=digitalRead(pulsador);
    if(valor==HIGH){
      digitalWrite(led,HIGH);
      delay(5);
      id_dedo=getFingerprintIDez();//Mira el Id de la huella reconocida
      
      if (id_dedo>0){
        //Serial.println("enroll finger print");
        
        
        contador=EEPROM.read(0+contador); //lee las direcciones ID asignadas para no sobre escribir
        contador++;
        getFingerprintEnroll();
        
        EEPROM.write(contador, contador);//Guarda el registro del ID de la ultima huella guardada, para una siguiente huella
        id_dedo=0;
        digitalWrite(led,LOW);
        delay(5);
      }
    }
  }
  analogValor=analogRead(batteria);// put your main code here, to run repeatedly:
  voltaje=0.0048*analogValor;
  
  if (voltaje<=minimo){
    digitalWrite(ledindicador,HIGH);
    delay(2);
  }
  else{
    digitalWrite(ledindicador,LOW);
    delay(2);
    
    
  }
  
  
}
//***Registra huellas**
uint8_t getFingerprintEnroll() {
    int p = -1;
    while (p != FINGERPRINT_OK) {
      p = finger.getImage();
      switch (p) {
      case FINGERPRINT_OK:
        
        break;
      case FINGERPRINT_NOFINGER:
        
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        
        break;
      case FINGERPRINT_IMAGEFAIL:
        
        break;
      default:
        
        break;
    }
  
  }
  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      
      break;
    case FINGERPRINT_IMAGEMESS:
      
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      
      return p;
    case FINGERPRINT_FEATUREFAIL:
      
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      
      return p;
    default:
      
      return p;
  }
  digitalWrite(led, HIGH);
  delay(20);
  digitalWrite(led, LOW);
  delay(10);
  digitalWrite(led, HIGH);
  delay(20);
  digitalWrite(led, LOW);
  delay(20);
  Serial.println("Enrollar");
  delay(5);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
 
  p = -1;
  
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
  
      break;
    case FINGERPRINT_NOFINGER:
  
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
  
      break;
    case FINGERPRINT_IMAGEFAIL:
  
      break;
    default:
  
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
  
      break;
    case FINGERPRINT_IMAGEMESS:
  
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
  
      return p;
    case FINGERPRINT_FEATUREFAIL:
  
      return p;
    case FINGERPRINT_INVALIDIMAGE:
  
      return p;
    default:
  
      return p;
  }

  // OK converted!
 

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
  
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
  
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
  
    return p;
  } else {
  
    return p;
  }

 
  p = finger.storeModel(contador); //Guarda la huella en la posicion de contador
  if (p == FINGERPRINT_OK) {
  
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
  
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
  
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
  
    return p;
  } else {
  
    return p;
  }

  return true;
}
//*Envia señal de apertura*
void abrirPuerta(){//Envia un 0 para abrir el candado
  
  delay(5);
  miBT.write('0');
  delay(20);
  miBT.write('1');
  delay(2);
  Serial.println("Abierto");
  delay(5);
  
  return;
}
///**no tuvo coincidencia la huella***
void Mal_Registro() {      // Activa un led 2 veces por huella no autorizada
  
  digitalWrite(led, HIGH);
  delay(20);
  digitalWrite(led, LOW);
  delay(10);
  digitalWrite(led, HIGH);
  delay(20);
  digitalWrite(led, LOW);
  getFingerprintIDez();
  delay(10);

  
}
//****Verifica huellas en la base de datos****
int getFingerprintIDez() {
  // returns -1 if failed, otherwise returns ID #
  
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) { 
    Mal_Registro();
    return -1;
  }

  // Si hay coincidencias de huella
  
  
  abrirPuerta();
  //Serial.print("BIENVENIDO "); Serial.print(finger.fingerID); 
  //if(finger.fingerID==1){
    // Serial.print("BIENVENIDO 1*** "); 
     //abrirPuerta();
   //}
  
  return finger.fingerID; 
}
