  /*** LIBRERIAS ***/
#include <DHT.h>
#include <Wire.h>              // LIBRERIA PARA LA COMUNICACION DE COMPONENTE I2C
#include <LiquidCrystal_I2C.h> // LIBRERIA PARA COMPONENTE I2C

  /*** INICIALIZACION DE LCD's 20X4 Y 16X2 CONFIGURACION DE LAS PANTALLAS ***/
       
                        /*** EQUIVALENCIA MODULO I2C ***/
                  /***  addr,en,rw,rs,d4,d5,d6,d7,bl,blpol***/
LiquidCrystal_I2C lcd2(0x26, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

  /*** DECLARACION DE VARIABLES ***/
  
/*** DETECTOR DE HUMO ***/
int sensorValue=0;
int extractor1 = 6;
int bombaIncendio = 10;

void setup() {
  lcd2.clear();
  lcd2.begin(20, 4);
  pinMode(extractor1, OUTPUT);
  pinMode(bombaIncendio, OUTPUT); 
}
/****** FUNCIONES HUMO ******/
void inicioHumo(){
  datoshumo();
  lcd2.setCursor(0, 0);
  lcd2.print("HUMO:");
  lcd2.setCursor(0, 1);
  lcd2.print("PPM CO:");
  lcd2.setCursor(0, 2);
  lcd2.print("EXTRACTOR:");
  lcd2.setCursor(0, 3);
  lcd2.print("ALARMA:");
}
void datoshumo(){
  lcd2.clear();
  lcd2.setCursor(8, 1);
  lcd2.print(sensorValue);
}
void detectado(){
  inicioHumo();
  digitalWrite(extractor1, HIGH);
  lcd2.setCursor(7, 0);
  lcd2.print("DETECTADO");
  lcd2.setCursor(10, 2);
  lcd2.print("ENCENDIDO");
  lcd2.setCursor(8, 3);
  lcd2.print(" ACTIVADA  ");
  sensorValue = analogRead(A5); 
}
void despejado(){
  inicioHumo();
  correccion();
  digitalWrite(extractor1, LOW);
  lcd2.setCursor(7, 0);
  lcd2.print("DESPEJADO");
  lcd2.setCursor(10, 2);
  lcd2.print(" APAGADO ");
  lcd2.setCursor(8, 3);
  lcd2.print("DESACTIVADA");
  sensorValue = analogRead(A5); 
}
void correccion(){
  inicioHumo();
  digitalWrite(extractor1, LOW);
  lcd2.setCursor(11, 1);
  lcd2.print(" ");
  lcd2.setCursor(7, 0);
  lcd2.print("DESPEJADO");
  lcd2.setCursor(10, 2);
  lcd2.print(" APAGADO ");
  lcd2.setCursor(8, 3);
  lcd2.print("DESACTIVADA");
  sensorValue = analogRead(A5); 
}

void loop() {
/****** FORMATO DE DATOS DE INICIO DE CADA PANTALLA ******/ 
  inicioHumo();
  
 /****** LECTURA ANALOGICA DE LOS SENSORES DE 
          TEMPERATURA Y HUMEDAD, HUMO, SUELO E
          ILUMINACION ******/
  
  sensorValue = analogRead(A5); 

   /****** FUNCIONAMIENTO DETECTOR DE HUMO ******/

  if(sensorValue < 1500 && sensorValue >= 400)
  {
    detectado();
  }
  else if (sensorValue < 400 && sensorValue >= 100)
  {
    despejado();
  }
  else if (sensorValue < 100 )
  {
    correccion();
  }
  delay(1000);
}
